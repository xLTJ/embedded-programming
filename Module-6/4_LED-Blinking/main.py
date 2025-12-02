from gpiozero import LED
from time import sleep

def main():
    led = LED(17)

    while True:
        led.on()
        sleep(1)
        led.off()
        sleep(1)


if __name__ == "__main__":
    main()