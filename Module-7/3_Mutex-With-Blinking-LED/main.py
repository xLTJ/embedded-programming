import threading
from gpiozero import LED, Button
from time import sleep

button = Button(2)
led = LED(22)

is_button_on = False
led_lock = threading.Lock()

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
            led_lock.acquire()
            led.on()
            while is_button_on:
                sleep(0.01) # stay here until the button is released
            led.off()
            led_lock.release()
        sleep(0.01)

def blink_worker():
    while True:
        # the lock means that if the button is pressed, it has to wait until its released
        led_lock.acquire()
        led.on()
        led_lock.release()
        sleep(2)
        led_lock.acquire()
        led.off()
        led_lock.release()
        sleep(2)


if __name__ == "__main__":
    main()