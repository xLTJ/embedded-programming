from smbus import SMBus
import time

ARDUINO_ADDR = 0x08
bus = SMBus(1)  # use I2C bus 1

def main():
    try:
        while True:
            print("\n1: Read potentiometer")
            print("2: LED ON")
            print("3: LED OFF")
            print("4: Exit")

            choice = input("\nChoice: ")

            match choice:
                case '1':
                    potentiometer_value = get_potentiometer_value()
                    print(f"Potentiometer: {potentiometer_value}")

                case '2':
                    set_led(True)
                    print("LED turned ON")

                case '3':
                    set_led(False)
                    print("LED turned OFF")

                case '4':
                    break

    finally:
        bus.close()


def get_potentiometer_value():
    # read potentiometer value from rrduino
    data = bus.read_i2c_block_data(ARDUINO_ADDR, 0, 2)
    value = (data[0] << 8) | data[1]  # combine two bytes
    return value


def set_led(state):
    # turn LED on (1) or off (0)
    if state:
        bus.write_byte(ARDUINO_ADDR, ord('1'))
    else:
        bus.write_byte(ARDUINO_ADDR, ord('0'))


if __name__ == "__main__":
    main()
