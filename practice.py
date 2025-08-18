import requests
import time
import matplotlib.pyplot as plt

ESP_IP = "http://192.168.0.105"

# Turn on interactive mode
plt.ion()

x_vals = []
y_vals = []
count = 0

fig, ax = plt.subplots()
line, = ax.plot([], [], 'bo-')  # blue dots with line

while True:
    try:
        response = requests.get(ESP_IP, timeout=5)
        if response.status_code == 200:
            distance = float(response.text.strip())
            print("Distance:", distance, "cm")

            # Append values
            x_vals.append(count)
            y_vals.append(distance)
            count += 1

            # Update plot
            line.set_xdata(x_vals)
            line.set_ydata(y_vals)
            ax.relim()
            ax.autoscale_view()

            plt.draw()
            plt.pause(0.01)

        else:
            print("Failed! Status code:", response.status_code)
    except requests.exceptions.RequestException as e:
        print("Error connecting to ESP8266:", e)

    time.sleep(0.5)
