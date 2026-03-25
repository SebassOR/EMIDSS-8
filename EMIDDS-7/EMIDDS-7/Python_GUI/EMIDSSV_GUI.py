import serial
import serial.tools.list_ports as list_ports
import PySimpleGUI as sg
from PIL import Image
import time
import os
import runpy
    
# Definir los valores por defecto
default_port_name = 'COM20'
default_baudrate = 9600

# Layout de la ventana de configuración inicial
layout_config = [
    [sg.Text('UART Configuration')],
    [sg.Text('Port:'), sg.InputText(default_port_name, key='-PORT_NAME-')],
    [sg.Text('Baudrate:'), sg.InputText(str(default_baudrate), key='-BAUDRATE-')],
    [sg.Multiline(size=(70, 5), key='-OUTPUT CONFIG-',text_color="black", autoscroll=True, reroute_stdout=True, reroute_stderr=True, echo_stdout_stderr=True) ],
    [sg.Button('Done'), sg.Button('See Ports', key = '-SEE PORTS-' )]
]

# Init Config Window
window_config = sg.Window('EMIDSS GUI Uart Config', layout_config, background_color='white', finalize=True)

# Bucle para la ventana de configuración inicial
while True:
    event, values = window_config.read()

    if event == sg.WIN_CLOSED or event == 'Done':
        break
    
    if event == sg.WIN_CLOSED or event == '-SEE PORTS-':
        # List all comports
        all_ports = list_ports.comports()
        print(all_ports)

        # Each entry in the `all_ports` list is a serial device. Check it's
        # description and device attributes to learn more
        first_serial_device = all_ports[0]
        print(first_serial_device.device)  # the `port_name`
        print(first_serial_device.description)  # perhaps helpful to know if this is your device
        

# Obtener los valores de configuración ingresados
port_name = values['-PORT_NAME-']
baudrate = int(values['-BAUDRATE-'])

# Cerrar la ventana de configuración inicial
window_config.close()

# Configuración de la conexión serial
"""ser = serial.Serial(
    port=port_name,
    baudrate=baudrate,
    bytesize=serial.EIGHTBITS,
    timeout=1
)"""
"""=========================================================="""
# Función para verificar y manejar la conexión serial
def check_serial_connection(port):
    try:
        ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            bytesize=serial.EIGHTBITS,
            timeout=1
        )
        if not ser.is_open:
            raise serial.SerialException('No se puede establecer la conexión serial.')
        return ser
    except serial.SerialException as e:
        sg.popup_error(f'{str(e)}')
        exit()

# Verificar y obtener la conexión serial
ser = check_serial_connection(port_name)  # Reemplaza 'COM10' con un puerto serial inválido para simular un error

    
"""==========================================================="""

# Resize the image
def resize_image(image_path, size, name):
    image = Image.open(image_path)
    image = image.resize(size, Image.LANCZOS)
    image.save(name + "_png.png")
    return name + "_png.png"

# Path to your image
emidss_path = 'EMIDS.png'
# Desired size (width, height)
emidss_size = (70, 70)  # Adjust the size as needed
emidss_png = resize_image(emidss_path, emidss_size, "emidss")

# Path to your image
send_path = 'send.png'
# Desired size (width, height)
send_size = (25, 25)  # Adjust the size as needed
send_png = resize_image(send_path, send_size, "send")

# Path to your image
exit_path = 'exit.png'
# Desired size (width, height)
exit_size = (25, 25)  # Adjust the size as needed
exit_png = resize_image(exit_path, exit_size, "exit")

"""====================================================== Layout configurations ============================================================="""
combo_read  = sg.Combo(['Time', 'Memory', 'SW Version'], font=('Arial Bold', 10), enable_events=True, size=(12,1),key='-COMBO READ-')
combo_write = sg.Combo(['Hour'], font=('Arial Bold', 10), enable_events=True,size=(12,1), key='-COMBO WRITE-')
combo_reset = sg.Combo(['Sensor', 'Memory'], font=('Arial Bold', 10), enable_events=True, size=(12,1), key='-COMBO RESET-')
blank_space_1 = sg.Text('                   ',background_color='white', key=None)
blank_space_2 = sg.Text('                   ',background_color='white', key=None)
Read_Service_Text  = sg.Text('Read Service',  font=("Verdana Bold",12), text_color="black", size=(25, 1), background_color='white')
Write_Service_Text = sg.Text('Write Service.   Bytes', font=("Verdana Bold",12), text_color="black", size=(30, 1), background_color='white')
Reset_Service_Text = sg.Text(' Reset Service', font=("Verdana Bold",12), text_color="black", size=(15, 1), background_color='white')
Raw_Message_Text = sg.Text('Raw Message', font=("Verdana Bold",12), text_color="black", size=(12, 1), background_color='white')
exit_button = sg.Button(image_filename='exit_png.png', key='-EXIT-')
port_button = sg.Button('Clear Output', key='-CLEAR OUTPUT-')
graph_button = sg.Button('Graph Data', key='-GRAPH BUTTON-')
Write_Input_1 = sg.Input(size=(2,1), key='-WRITE IN 1-')
Write_Input_2 = sg.Input(size=(2,1), key='-WRITE IN 2-')
info_button = sg.Button('Info', key='-INFO BUTTON-')

# Layout of the window
layout = [
    [sg.Text('',background_color='white', size=(33,1)), sg.Image(filename=emidss_png, key='-IMAGE-'), sg.Text('', font=("Verdana", 20), text_color="blue", size=(15, 1), background_color='white')],
    [Read_Service_Text, Write_Service_Text, Reset_Service_Text],
    [combo_read, sg.Button(image_filename='send_png.png', border_width=None, key='-SEND READ-'), blank_space_1,  combo_write, Write_Input_1, Write_Input_2, sg.Button(image_filename='send_png.png', border_width=None, key='-SEND WRITE-'), blank_space_2,  combo_reset, sg.Button(image_filename='send_png.png', border_width=None, key='-SEND RESET-')],
    [sg.Text('________________________________________________________________________', font=("Verdana Bold",12), text_color="black", size=(72, 1), background_color='white')],
    [Raw_Message_Text, sg.Input(size=(20,1), key='-COMMAND-'), sg.Button(image_filename='send_png.png', border_width=None, key='-SEND RAW-'), sg.Text('',background_color='white', size=(27,1)), graph_button, port_button],
    [sg.Multiline(size=(100, 20), key='-OUTPUT-',text_color="green", autoscroll=True, reroute_stdout=True, reroute_stderr=True, echo_stdout_stderr=True) ],
    [info_button,sg.Text('',background_color='white', size=(80,1)), exit_button,]
]

"""====================================================== APPLICATION ============================================================="""

# Create the window
window = sg.Window("EMIDSS-V GUI", layout, background_color='white', finalize=True)

# Event loop
while True:
    event, values = window.read(timeout=30)
    
    if event == sg.WIN_CLOSED or event == '-EXIT-':
        break
    """============= READ SERVICES ==============="""        
    if event == '-SEND READ-':
        read_action = values['-COMBO READ-']
        
        if read_action == 'Time':
            print('Reading Time')
            
            ser.write(b"S2201")
            ser.write(b"S2201")
            
        elif read_action == 'Memory':
            print('Reading Memory')
            
            ser.write(b"S2202")
            ser.write(b"S2202")
            
        elif read_action == 'SW Version':
            print('Reading SW Version')
            
            ser.write(b"S2203")
            ser.write(b"S2203")

    """============= WRITE SERVICES ==============="""  
    if event == '-SEND WRITE-':
        read_action = values['-COMBO WRITE-']
        read_input_1 = values['-WRITE IN 1-']
        read_input_2 = values['-WRITE IN 2-']
        
        if read_action == 'Hour':
            print('Write Time [' + read_input_1 + ':' + read_input_2 + ']')
            
            ser.write(("S2301" + read_input_1 + read_input_2).encode('utf-8'))
            ser.write(("S2301" + read_input_1 + read_input_2).encode('utf-8'))
    
    """============= RESET SERVICES ==============="""          
    if event == '-SEND RESET-':
        read_action = values['-COMBO RESET-']
        
        if read_action == 'Sensor':
            print('Reset Sensor...')
            ser.write(b"S1101")
            ser.write(b"S1101")
            
        elif read_action == 'Memory':
            print('Reset Memory...')
            ser.write(b"S1102")
            ser.write(b"S1102")
            
    """============= RAW SERVICES ==============="""             
    if event == '-SEND RAW-':
        command = values['-COMMAND-']
        print('Raw Command Sent: ' + command)
        ser.write((command).encode('utf-8'))
        ser.write((command).encode('utf-8'))
     
    """============= GET PORTS BUTTON ==============="""   
    if event == '-CLEAR OUTPUT-':
        window['-OUTPUT-'].update('')
    
    """============= READ UART FUNCTION ==============="""    
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        #print(f'{line}')
        window['-OUTPUT-'].print(line, text_color='blue')
        time.sleep(0.01)# Pausa para evitar una sobrecarga de la CPU
        
    """ Create Graph"""
    if event == '-GRAPH BUTTON-':
        runpy.run_path('GraphicData.py')   
        
    if event == '-INFO BUTTON-':
        sg.popup('How To Use this GUI.',
                 '1. How to graph data:\n'
                 '    Fill EMIDSS_V_Data.xlsx with the data obtained from\n' 
                 '    the EMIDSS memory data.'
                )
    
"""================================================================================================================================"""
          
window.close()
ser.close()