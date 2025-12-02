import threading
from gpiozero import LED, Button
from time import sleep

button = Button(2)
led = LED(22)

is_button_on = False

def main():
    led_thead = threading.Thread(target=led_worker)
    button_thread = threading.Thread(target=button_worker)
    blink_thread = threading.Thread(target=blink_worker)
    led_thead.start()
    button_thread.start()
    blink_thread.start()


def button_worker():
    global is_button_on

    # i would probably use events for this normally, but since we have to use a thread...
    while True:
        if button.is_active:
            is_button_on = True
        else:
            is_button_on = False
        sleep(0.01)


def led_worker():
    while True:
        if is_button_on:
            led.on()
        else:
            led.off()
        sleep(0.01)

def blink_worker():
    while True:
        led.on()
        sleep(2)
        led.off()
        sleep(2)


if __name__ == "__main__":
    main()