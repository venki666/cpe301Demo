import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext
import serial.tools.list_ports
from serial import Serial
import xml.etree.ElementTree as ET
import csv
import threading
import datetime

class SerialMonitor:
    def __init__(self, master):
        self.master = master
        self.master.title("Serial Monitor")
        self.master.geometry("800x600")

        self.create_widgets()

        # Flag to indicate if the serial connection is active
        self.connection_active = False

    def create_widgets(self):
        self.port_combobox_label = ttk.Label(self.master, text="Select Port:")
        self.port_combobox_label.grid(row=0, column=0, padx=10, pady=10)

        self.populate_ports()

        self.baud_combobox_label = ttk.Label(self.master, text="Select Baud Rate:")
        self.baud_combobox_label.grid(row=0, column=1, padx=10, pady=10)

        self.baud_combobox = ttk.Combobox(self.master, values=["2400","4800","9600","14400", "115200"], state="readonly")
        self.baud_combobox.set("9600")
        self.baud_combobox.grid(row=0, column=2, padx=10, pady=10)

        self.connect_button = ttk.Button(self.master, text="Connect", command=self.connect)
        self.connect_button.grid(row=0, column=3, padx=10, pady=10)

        self.disconnect_button = ttk.Button(self.master, text="Disconnect", command=self.disconnect, state=tk.DISABLED)
        self.disconnect_button.grid(row=0, column=4, padx=10, pady=10)

        self.export_txt_button = ttk.Button(self.master, text="Export as TXT", command=self.export_txt, state=tk.DISABLED)
        self.export_txt_button.grid(row=0, column=5, padx=10, pady=10)

        self.export_csv_button = ttk.Button(self.master, text="Export as CSV", command=self.export_csv, state=tk.DISABLED)
        self.export_csv_button.grid(row=0, column=6, padx=10, pady=10)

        self.export_xml_button = ttk.Button(self.master, text="Export as XML", command=self.export_xml, state=tk.DISABLED)
        self.export_xml_button.grid(row=0, column=7, padx=10, pady=10)

        self.log_text = scrolledtext.ScrolledText(self.master, wrap=tk.WORD, width=80, height=20)
        self.log_text.grid(row=1, column=0, columnspan=8, padx=10, pady=10)

    def populate_ports(self):
        ports = [port.device for port in serial.tools.list_ports.comports()]
        self.port_combobox = ttk.Combobox(self.master, values=ports, state="readonly")
        self.port_combobox.grid(row=0, column=0, padx=10, pady=10)

    def connect(self):
        port = self.port_combobox.get()
        baud = int(self.baud_combobox.get())
        try:
            self.ser = Serial(port, baud, timeout=1)
            self.log_text.delete(1.0, tk.END)
            self.log_text.insert(tk.END, f"Connected to {port} at {baud} baud\n")
            self.disconnect_button["state"] = tk.NORMAL
            self.connect_button["state"] = tk.DISABLED
            self.export_txt_button["state"] = tk.NORMAL
            self.export_csv_button["state"] = tk.NORMAL
            self.export_xml_button["state"] = tk.NORMAL

            self.connection_active = True

            self.thread = threading.Thread(target=self.read_from_port)
            self.thread.start()
        except Exception as e:
            self.log_text.insert(tk.END, f"Error: {str(e)}\n")

    def disconnect(self):
        self.connection_active = False  # Set the flag to False to stop the reading thread
        if hasattr(self, 'ser') and self.ser.is_open:
            self.ser.close()
        self.connect_button["state"] = tk.NORMAL
        self.disconnect_button["state"] = tk.DISABLED
        self.export_txt_button["state"] = tk.DISABLED
        self.export_csv_button["state"] = tk.DISABLED
        self.export_xml_button["state"] = tk.DISABLED
        self.log_text.insert(tk.END, "Disconnected\n")

    def read_from_port(self):
        while self.connection_active:  # Check the flag in the reading loop
            try:
                line = self.ser.readline().decode("utf-8")
                if line:
                    self.log_text.insert(tk.END, line)
                    self.log_text.see(tk.END)
            except Exception as e:
                if self.connection_active:  # Only log errors if the connection is still active
                    self.log_text.insert(tk.END, f"Error reading from port: {str(e)}\n")
                break

    def export_txt(self):
        data = self.log_text.get(1.0, tk.END)
        filename = f"serial_log_{datetime.datetime.now().strftime('%Y%m%d%H%M%S')}.txt"
        with open(filename, "w") as file:
            file.write(data)
        self.log_text.insert(tk.END, f"Log exported as TXT: {filename}\n")

    def export_csv(self):
        data = self.log_text.get(1.0, tk.END)
        filename = f"serial_log_{datetime.datetime.now().strftime('%Y%m%d%H%M%S')}.csv"
        with open(filename, "w", newline="") as file:
            writer = csv.writer(file)
            writer.writerows([line.split() for line in data.splitlines()])
        self.log_text.insert(tk.END, f"Log exported as CSV: {filename}\n")

    def export_xml(self):
        data = self.log_text.get(1.0, tk.END)
        filename = f"serial_log_{datetime.datetime.now().strftime('%Y%m%d%H%M%S')}.xml"
        root = ET.Element("LogData")
        lines = data.splitlines()
        for line in lines:
            entry = ET.SubElement(root, "Entry")
            ET.SubElement(entry, "Data").text = line
        tree = ET.ElementTree(root)
        tree.write(filename)
        self.log_text.insert(tk.END, f"Log exported as XML: {filename}\n")

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialMonitor(root)
    root.mainloop()
