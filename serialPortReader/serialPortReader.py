import serial

# Define the serial port name and baud rate
port = 'COM7'  # Replace with the appropriate serial port
baud_rate = 115200  # Match the baud rate with the Arduino sketch

# Open the serial port
try:
    ser = serial.Serial(port, baud_rate)
except serial.SerialException as e:
    print(f"Failed to open serial port: {e}")
    exit()

# Open a file for writing the received data
filename = 'data.txt'  # Replace with the desired filename
with open(filename, 'w') as file:
    try:
        # Read and write data until interrupted
        i = 0
        while True:
            # Read a line from the serial port
            line = ser.readline().decode().strip()
            if line:
                try:
                    value = int(line)
                    # Write the line to the file
                    file.write(line + '\n')
                    file.flush()  # Flush the buffer to ensure data is written immediately
                except ValueError:
                    pass
            i += 1
            # Print the line to the console
            # print(line)
    except KeyboardInterrupt:
        pass

# Close the serial port
ser.close()
