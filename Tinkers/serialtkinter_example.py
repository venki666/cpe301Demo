import serial 
import tkinter as tk

serialPort = "COM6"
baudRate = 9600
ser = serial.Serial(serialPort , baudRate, timeout=1, writeTimeout=0) #ensure non-blocking

#make a TkInter Window
root = tk.Tk()
root.title("Serial Data")

text_area = tk.Text(root)
text_area.pack(fill=tk.BOTH, expand=True)
#make our own buffer
#useful for parsing commands
#Serial.readline seems unreliable at times too
serBuffer = ""

def read_serial():
        if ser.in_waiting > 0:
            try:
                data = ser.readline().decode('utf-8').strip()
                text_area.insert(tk.END, data + '\n')
                text_area.see(tk.END)  # Auto-scroll to the bottom
            except UnicodeDecodeError:
                pass  # Handle potential decoding errors
        root.after(100, read_serial)  # Check for new data every 100 ms

# after initializing serial, an arduino may need a bit of time to reset
read_serial()  # Start reading serial data

root.mainloop()