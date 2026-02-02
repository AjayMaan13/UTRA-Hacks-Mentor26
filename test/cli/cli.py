"""
Arduino Serial Controller & Visualizer - Terminal Edition
Terminal-based control and monitoring using curses
"""

import serial
import serial.tools.list_ports
import curses
import threading
import time
import queue
from collections import deque

class ArduinoTerminalController:
    def __init__(self):
        self.serial_port = None
        self.connected = False
        self.running = False
        self.data_queue = queue.Queue()
        
        # Sensor data storage
        self.color_data = {'R': 0, 'G': 0, 'B': 0}
        self.ir_data = {'IR1': 0, 'IR2': 0}  # Analog values (0-4095)
        self.ir_threshold = 2000  # Detection threshold
        self.distance = 0.0
        
        # Motor states
        self.motor_a_speed = 150
        self.motor_b_speed = 150
        self.motor_both_speed = 150
        
        # Servo states
        self.servo1_angle = 90
        self.servo2_angle = 90
        
        # History for graphs (last 50 readings)
        self.color_history = {'R': deque(maxlen=50), 'G': deque(maxlen=50), 'B': deque(maxlen=50)}
        self.distance_history = deque(maxlen=50)
        self.ir1_history = deque(maxlen=50)
        self.ir2_history = deque(maxlen=50)
        
        # Auto-refresh
        self.auto_refresh = False
        
        # Status messages
        self.status_msg = "Disconnected"
        self.last_command = ""
    
    def get_available_ports(self):
        """Get list of available serial ports"""
        ports = serial.tools.list_ports.comports()
        return [port.device for port in ports]
    
    def connect(self, port):
        """Connect to Arduino"""
        try:
            self.serial_port = serial.Serial(port, 9600, timeout=1)
            time.sleep(2)  # Wait for Arduino reset
            self.connected = True
            self.running = True
            self.status_msg = f"Connected to {port}"
            
            # Start reading thread
            self.read_thread = threading.Thread(target=self.read_serial, daemon=True)
            self.read_thread.start()
            
            return True
        except Exception as e:
            self.status_msg = f"Connection error: {str(e)}"
            return False
    
    def disconnect(self):
        """Disconnect from Arduino"""
        self.running = False
        self.connected = False
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()
        self.status_msg = "Disconnected"
    
    def send_command(self, command):
        """Send command to Arduino"""
        if self.connected and self.serial_port:
            try:
                self.serial_port.write(f"{command}\n".encode())
                self.last_command = command
                time.sleep(0.05)
            except Exception as e:
                self.status_msg = f"Send error: {str(e)}"
        else:
            self.status_msg = "Not connected!"
    
    def read_serial(self):
        """Read serial data from Arduino"""
        while self.running:
            if self.serial_port and self.serial_port.in_waiting:
                try:
                    line = self.serial_port.readline().decode('utf-8', errors='ignore').strip()
                    if line:
                        self.process_serial_data(line)
                except Exception as e:
                    pass
            time.sleep(0.05)
    
    def process_serial_data(self, line):
        """Process incoming serial data"""
        # Parse color sensor data
        if line.startswith("  R:"):
            try:
                parts = line.split()
                self.color_data['R'] = int(parts[1])
                self.color_data['G'] = int(parts[3])
                self.color_data['B'] = int(parts[5])
                
                # Update history
                self.color_history['R'].append(self.color_data['R'])
                self.color_history['G'].append(self.color_data['G'])
                self.color_history['B'].append(self.color_data['B'])
            except (ValueError, IndexError):
                pass
        
        # Parse IR sensor data (analog values)
        elif "IR1:" in line and "IR2:" in line:
            try:
                # Parse format: "IR1: 1234 (30%) [DETECTED]  |  IR2: 3456 (84%) [CLEAR]"
                parts = line.split("IR1:")[1].split("IR2:")
                ir1_part = parts[0].strip()
                ir2_part = parts[1].strip() if len(parts) > 1 else ""
                
                # Extract numeric value (first number after IR1:)
                ir1_val = int(ir1_part.split()[0])
                ir2_val = int(ir2_part.split()[0]) if ir2_part else 0
                
                self.ir_data['IR1'] = ir1_val
                self.ir_data['IR2'] = ir2_val
                
                # Store normalized values (0-1) for history/graphs
                self.ir1_history.append(ir1_val / 4095.0)
                self.ir2_history.append(ir2_val / 4095.0)
            except (ValueError, IndexError):
                pass
        
        # Parse ultrasonic data
        elif line.startswith("Distance:"):
            try:
                distance_str = line.split("cm")[0].split(":")[-1].strip()
                self.distance = float(distance_str)
                self.distance_history.append(self.distance)
            except (ValueError, IndexError):
                pass

def safe_addstr(stdscr, y, x, text, attr=0):
    """Safely add string to screen, checking bounds"""
    try:
        max_y, max_x = stdscr.getmaxyx()
        if 0 <= y < max_y and 0 <= x < max_x:
            # Truncate text if it would exceed screen width
            available_width = max_x - x
            if available_width > 0:
                text = text[:available_width]
                stdscr.addstr(y, x, text, attr)
    except (curses.error, ValueError):
        pass  # Silently ignore out-of-bounds errors

def draw_box(stdscr, y, x, height, width, title=""):
    """Draw a box with optional title"""
    max_y, max_x = stdscr.getmaxyx()
    
    # Adjust dimensions to fit screen
    if y >= max_y or x >= max_x:
        return
    if y + height > max_y:
        height = max_y - y
    if x + width > max_x:
        width = max_x - x
    
    if height < 2 or width < 2:
        return
    
    try:
        # Draw corners and borders
        stdscr.addch(y, x, curses.ACS_ULCORNER)
        if x + width - 1 < max_x:
            stdscr.addch(y, x + width - 1, curses.ACS_URCORNER)
        if y + height - 1 < max_y:
            stdscr.addch(y + height - 1, x, curses.ACS_LLCORNER)
            if x + width - 1 < max_x:
                stdscr.addch(y + height - 1, x + width - 1, curses.ACS_LRCORNER)
        
        for i in range(1, width - 1):
            if x + i < max_x:
                stdscr.addch(y, x + i, curses.ACS_HLINE)
                if y + height - 1 < max_y:
                    stdscr.addch(y + height - 1, x + i, curses.ACS_HLINE)
        
        for i in range(1, height - 1):
            if y + i < max_y:
                stdscr.addch(y + i, x, curses.ACS_VLINE)
                if x + width - 1 < max_x:
                    stdscr.addch(y + i, x + width - 1, curses.ACS_VLINE)
        
        # Add title
        if title and x + 2 < max_x:
            safe_addstr(stdscr, y, x + 2, f" {title} ", curses.A_BOLD)
    except (curses.error, ValueError):
        pass

def draw_bar_graph(stdscr, y, x, width, value, max_value, label=""):
    """Draw a horizontal bar graph"""
    max_y, max_x = stdscr.getmaxyx()
    if y >= max_y or x >= max_x:
        return
    
    if max_value > 0:
        bar_width = int((value / max_value) * (width - len(label) - 5))
    else:
        bar_width = 0
    
    safe_addstr(stdscr, y, x, label)
    if x + len(label) + 1 < max_x:
        safe_addstr(stdscr, y, x + len(label) + 1, "█" * bar_width)
    if x + width - 4 < max_x:
        safe_addstr(stdscr, y, x + width - 4, f"{int(value):>3}")

def draw_line_graph(stdscr, y, x, width, height, data, label="", max_val=None):
    """Draw a simple line graph using characters"""
    if len(data) == 0:
        return
    
    max_y, max_x = stdscr.getmaxyx()
    if y >= max_y or x >= max_x:
        return
    
    # Determine max value for scaling
    if max_val is None:
        max_val = max(data) if max(data) > 0 else 1
    
    # Draw label
    safe_addstr(stdscr, y, x, label)
    
    # Draw graph
    for i, val in enumerate(list(data)):
        if i >= width or x + i >= max_x:
            break
        
        # Scale value to height
        scaled = int((val / max_val) * (height - 1))
        char_y = y + height - scaled - 1
        
        if 0 <= char_y < max_y and y <= char_y < y + height:
            try:
                stdscr.addch(char_y, x + i, '█')
            except (curses.error, ValueError):
                pass

def main(stdscr):
    # Setup curses
    curses.curs_set(0)  # Hide cursor
    stdscr.nodelay(1)   # Non-blocking input
    stdscr.timeout(100) # 100ms timeout
    
    # Initialize colors
    curses.start_color()
    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_YELLOW, curses.COLOR_BLACK)
    curses.init_pair(4, curses.COLOR_CYAN, curses.COLOR_BLACK)
    curses.init_pair(5, curses.COLOR_MAGENTA, curses.COLOR_BLACK)
    
    controller = ArduinoTerminalController()
    
    # Connection phase
    ports = controller.get_available_ports()
    if not ports:
        safe_addstr(stdscr, 0, 0, "No serial ports found! Press any key to exit.")
        stdscr.getch()
        return
    
    # Port selection
    selected_port = 0
    while True:
        stdscr.clear()
        max_y, max_x = stdscr.getmaxyx()
        safe_addstr(stdscr, 0, 0, "=== Arduino Terminal Controller ===", curses.A_BOLD)
        safe_addstr(stdscr, 2, 0, "Select COM Port (↑/↓ arrows, Enter to connect):")
        
        for i, port in enumerate(ports):
            y_pos = 4 + i
            if y_pos < max_y - 1:
                if i == selected_port:
                    safe_addstr(stdscr, y_pos, 2, f"> {port}", curses.A_REVERSE)
                else:
                    safe_addstr(stdscr, y_pos, 2, f"  {port}")
        
        quit_y = 6 + len(ports)
        if quit_y < max_y - 1:
            safe_addstr(stdscr, quit_y, 0, "Press 'q' to quit")
        stdscr.refresh()
        
        key = stdscr.getch()
        if key == curses.KEY_UP:
            selected_port = max(0, selected_port - 1)
        elif key == curses.KEY_DOWN:
            selected_port = min(len(ports) - 1, selected_port + 1)
        elif key == ord('\n'):
            if controller.connect(ports[selected_port]):
                break
            else:
                stdscr.addstr(8 + len(ports), 0, controller.status_msg, curses.color_pair(2))
                stdscr.refresh()
                time.sleep(2)
        elif key == ord('q'):
            return
    
    # Main control loop
    selected_option = 0
    menu_options = [
        "Motor A Forward", "Motor A Backward", "Motor B Forward", "Motor B Backward",
        "Both Forward", "Both Backward", "STOP ALL",
        "Servo 1 -10°", "Servo 1 +10°", "Servo 2 -10°", "Servo 2 +10°",
        "Read Color", "Read IR", "Read Distance", "Toggle Auto-refresh",
        "Quit"
    ]
    
    last_auto_refresh = time.time()
    
    while True:
        stdscr.clear()
        height, width = stdscr.getmaxyx()
        
        # Header
        safe_addstr(stdscr, 0, 0, "=== Arduino Terminal Controller ===", curses.A_BOLD | curses.color_pair(4))
        status_color = curses.color_pair(1) if controller.connected else curses.color_pair(2)
        status_x = max(0, width - len(controller.status_msg) - 1)
        safe_addstr(stdscr, 0, status_x, controller.status_msg, status_color)
        
        # Left column - Controls
        draw_box(stdscr, 2, 0, height - 3, 40, "Controls")
        
        y_pos = 3
        max_y, max_x = stdscr.getmaxyx()
        controls_box_height = height - 3
        
        # Only draw menu options that fit in the available space
        for i, option in enumerate(menu_options):
            if y_pos >= max_y - 1 or y_pos >= 2 + controls_box_height - 1:
                break
            if i == selected_option:
                safe_addstr(stdscr, y_pos, 2, f"> {option}", curses.A_REVERSE)
            else:
                safe_addstr(stdscr, y_pos, 2, f"  {option}")
            y_pos += 1
        
        # Motor speeds (only if there's space)
        if y_pos + 1 < max_y - 1 and y_pos + 1 < 2 + controls_box_height - 1:
            safe_addstr(stdscr, y_pos + 1, 2, f"Motor A Speed: {controller.motor_a_speed}")
        if y_pos + 2 < max_y - 1 and y_pos + 2 < 2 + controls_box_height - 1:
            safe_addstr(stdscr, y_pos + 2, 2, f"Motor B Speed: {controller.motor_b_speed}")
        if y_pos + 3 < max_y - 1 and y_pos + 3 < 2 + controls_box_height - 1:
            safe_addstr(stdscr, y_pos + 3, 2, f"Both Motors: {controller.motor_both_speed}")
        
        # Servo positions (only if there's space)
        if y_pos + 5 < max_y - 1 and y_pos + 5 < 2 + controls_box_height - 1:
            safe_addstr(stdscr, y_pos + 5, 2, f"Servo 1: {controller.servo1_angle}°")
        if y_pos + 6 < max_y - 1 and y_pos + 6 < 2 + controls_box_height - 1:
            safe_addstr(stdscr, y_pos + 6, 2, f"Servo 2: {controller.servo2_angle}°")
        
        # Auto-refresh status (only if there's space)
        if y_pos + 8 < max_y - 1 and y_pos + 8 < 2 + controls_box_height - 1:
            auto_status = "ON" if controller.auto_refresh else "OFF"
            auto_color = curses.color_pair(1) if controller.auto_refresh else curses.color_pair(2)
            safe_addstr(stdscr, y_pos + 8, 2, f"Auto-refresh: {auto_status}", auto_color)
        
        # Right column - Sensor Data
        draw_box(stdscr, 2, 42, 15, 38, "Sensor Readings")
        
        # Color sensor
        safe_addstr(stdscr, 3, 44, "Color Sensor (RGB):", curses.A_BOLD)
        draw_bar_graph(stdscr, 4, 44, 34, controller.color_data['R'], 300, "R:")
        draw_bar_graph(stdscr, 5, 44, 34, controller.color_data['G'], 300, "G:")
        draw_bar_graph(stdscr, 6, 44, 34, controller.color_data['B'], 300, "B:")
        
        # IR sensors (analog values)
        safe_addstr(stdscr, 8, 44, "IR Sensors (Analog):", curses.A_BOLD)
        ir1_detected = controller.ir_data['IR1'] < controller.ir_threshold
        ir2_detected = controller.ir_data['IR2'] < controller.ir_threshold
        ir1_status = "DETECTED" if ir1_detected else "CLEAR"
        ir2_status = "DETECTED" if ir2_detected else "CLEAR"
        ir1_color = curses.color_pair(2) if ir1_detected else curses.color_pair(1)
        ir2_color = curses.color_pair(2) if ir2_detected else curses.color_pair(1)
        
        ir1_percent = int((controller.ir_data['IR1'] / 4095.0) * 100)
        ir2_percent = int((controller.ir_data['IR2'] / 4095.0) * 100)
        safe_addstr(stdscr, 9, 44, f"IR1: {controller.ir_data['IR1']} ({ir1_percent}%) {ir1_status}", ir1_color)
        safe_addstr(stdscr, 10, 44, f"IR2: {controller.ir_data['IR2']} ({ir2_percent}%) {ir2_status}", ir2_color)
        
        # Ultrasonic
        safe_addstr(stdscr, 12, 44, "Ultrasonic Distance:", curses.A_BOLD)
        safe_addstr(stdscr, 13, 44, f"{controller.distance:.1f} cm", curses.color_pair(5))
        
        # Bottom - Graphs
        graph_y = 17
        if height > 30:
            draw_box(stdscr, graph_y, 0, height - graph_y - 1, width, "Real-time Data")
            
            graph_height = 5
            
            # Color history graph
            if len(controller.color_history['R']) > 0:
                safe_addstr(stdscr, graph_y + 1, 2, "Color (R/G/B):", curses.A_BOLD)
                draw_line_graph(stdscr, graph_y + 2, 2, 50, graph_height, 
                               controller.color_history['R'], max_val=400)
            
            # Distance history graph
            if len(controller.distance_history) > 0:
                safe_addstr(stdscr, graph_y + 1, 55, "Distance:", curses.A_BOLD)
                draw_line_graph(stdscr, graph_y + 2, 55, 50, graph_height,
                               controller.distance_history, max_val=100)
        
        # Instructions
        safe_addstr(stdscr, height - 1, 0, "↑/↓: Navigate | Enter: Execute | +/-: Adjust Speed | q: Quit", 
                     curses.color_pair(3))
        
        stdscr.refresh()
        
        # Auto-refresh
        if controller.auto_refresh and (time.time() - last_auto_refresh) > 1.0:
            controller.send_command("status")
            last_auto_refresh = time.time()
        
        # Handle input
        key = stdscr.getch()
        
        if key == curses.KEY_UP:
            selected_option = max(0, selected_option - 1)
        elif key == curses.KEY_DOWN:
            selected_option = min(len(menu_options) - 1, selected_option + 1)
        elif key == ord('+') or key == ord('='):
            if selected_option < 2:  # Motor A
                controller.motor_a_speed = min(255, controller.motor_a_speed + 10)
            elif selected_option < 4:  # Motor B
                controller.motor_b_speed = min(255, controller.motor_b_speed + 10)
            elif selected_option < 7:  # Both motors
                controller.motor_both_speed = min(255, controller.motor_both_speed + 10)
        elif key == ord('-') or key == ord('_'):
            if selected_option < 2:  # Motor A
                controller.motor_a_speed = max(0, controller.motor_a_speed - 10)
            elif selected_option < 4:  # Motor B
                controller.motor_b_speed = max(0, controller.motor_b_speed - 10)
            elif selected_option < 7:  # Both motors
                controller.motor_both_speed = max(0, controller.motor_both_speed - 10)
        elif key == ord('\n'):
            # Execute selected option
            if selected_option == 0:  # Motor A Forward
                controller.send_command(f"motor A {controller.motor_a_speed} F")
            elif selected_option == 1:  # Motor A Backward
                controller.send_command(f"motor A {controller.motor_a_speed} B")
            elif selected_option == 2:  # Motor B Forward
                controller.send_command(f"motor B {controller.motor_b_speed} F")
            elif selected_option == 3:  # Motor B Backward
                controller.send_command(f"motor B {controller.motor_b_speed} B")
            elif selected_option == 4:  # Both Forward
                controller.send_command(f"motors {controller.motor_both_speed} F")
            elif selected_option == 5:  # Both Backward
                controller.send_command(f"motors {controller.motor_both_speed} B")
            elif selected_option == 6:  # STOP ALL
                controller.send_command("stop")
            elif selected_option == 7:  # Servo 1 -10
                controller.servo1_angle = max(0, controller.servo1_angle - 10)
                controller.send_command(f"servo 1 {controller.servo1_angle}")
            elif selected_option == 8:  # Servo 1 +10
                controller.servo1_angle = min(180, controller.servo1_angle + 10)
                controller.send_command(f"servo 1 {controller.servo1_angle}")
            elif selected_option == 9:  # Servo 2 -10
                controller.servo2_angle = max(0, controller.servo2_angle - 10)
                controller.send_command(f"servo 2 {controller.servo2_angle}")
            elif selected_option == 10:  # Servo 2 +10
                controller.servo2_angle = min(180, controller.servo2_angle + 10)
                controller.send_command(f"servo 2 {controller.servo2_angle}")
            elif selected_option == 11:  # Read Color
                controller.send_command("color")
            elif selected_option == 12:  # Read IR
                controller.send_command("ir")
            elif selected_option == 13:  # Read Distance
                controller.send_command("ultra")
            elif selected_option == 14:  # Toggle Auto-refresh
                controller.auto_refresh = not controller.auto_refresh
            elif selected_option == 15:  # Quit
                break
        elif key == ord('q'):
            break
        
        time.sleep(0.05)
    
    # Cleanup
    controller.disconnect()

if __name__ == "__main__":
    curses.wrapper(main)