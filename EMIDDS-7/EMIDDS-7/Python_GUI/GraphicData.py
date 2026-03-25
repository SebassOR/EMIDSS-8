import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime

# Función para leer datos de Excel y combinar columnas de hora y minutos
def read_data_from_excel(file_path):
    df = pd.read_excel(file_path)
    df['Time'] = df.apply(lambda row: datetime.datetime.combine(datetime.date.today(), datetime.time(int(row['Hour']), int(row['Min']))), axis=1)
    return df

# Archivo Excel de ejemplo
excel_file = 'EMIDSS_V_Data.xlsx'  # Reemplaza con el nombre de tu archivo Excel
data = read_data_from_excel(excel_file)

# Limpiar los datos (interpolación de valores faltantes)
data = data.interpolate()

# variables to graph
time_data = data['Time']
temperature_data = data['Temperature']
humidity_data = data['Humity']
pressure_data = data['Pressure']

# create figs and plots
fig, axs = plt.subplots(3, 1, figsize=(10, 15))

# time format
time_format = mdates.DateFormatter('%H:%M')

# temp graph
axs[0].plot(time_data, temperature_data, label='Temperatura', color='r')
axs[0].set_title('EMIDSS-V Mission Data')
axs[0].set_xlabel('Tiempo')
axs[0].set_ylabel('Temperatura')
axs[0].xaxis.set_major_formatter(time_format)
axs[0].legend()

# hum graph
axs[1].plot(time_data, humidity_data, label='Humedad', color='b')
axs[1].set_title('Humedad en función del tiempo')
axs[1].set_xlabel('Tiempo')
axs[1].set_ylabel('Humedad')
axs[1].xaxis.set_major_formatter(time_format)
axs[1].legend()

# press graph
axs[2].plot(time_data, pressure_data, label='Presion', color='g')
axs[2].set_title('Presión en función del tiempo')
axs[2].set_xlabel('Tiempo')
axs[2].set_ylabel('Presión')
axs[2].xaxis.set_major_formatter(time_format)
axs[2].legend()

# Adjust layout
plt.tight_layout()

# Print graphic
plt.show()
