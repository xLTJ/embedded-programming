from gpiozero import LED
from time import sleep
from multiprocessing import shared_memory as sm

led_list = [LED(17), LED(27), LED(22)]

def main():
    state = {'is_locked': False} # global variables are stinky
    commandShm = sm.SharedMemory(name='commands', create=True, size=16)
    responseShm = sm.SharedMemory(name='responses', create=True, size=64)

    try:
        while True:
            # the first byte is used to show if its a new command or not
            if commandShm.buf[0] == 1:
                data = commandShm.buf[1:].tobytes()
                command = bytes(data).decode('utf-8').strip('\x00')

                response = handle_command(command, state).encode('utf-8')
                responseShm.buf[:] = b'\x00' * len(responseShm.buf) # reset buffer
                responseShm.buf[1:1+len(response)] = response
                responseShm.buf[0] = 1 # set flag for new response

                # reset the first byte to show the command has been processed
                commandShm.buf[0] = 0

            sleep(0.1)
    finally:
        commandShm.close()
        commandShm.unlink()


def handle_command(command, state):
    try:
        mainCommand, argument = command.split(':')
        match mainCommand:
            case 'T':
                if state['is_locked']:
                    return 'LEDs are locked'
                else:
                    led_list[int(argument)-1].on()
                return 'OK'

            case 'S':
                if state['is_locked']:
                    return 'LEDs are locked'
                else:
                    led_list[int(argument)-1].off()
                return 'OK'

            case 'B':
                return str(led_list[int(argument)-1].is_active)

            case 'SBL':
                if argument == '1':
                    state['is_locked'] = False
                    return 'OK'
                elif argument == '0':
                    state['is_locked'] = True
                    return 'OK'
                return 'Error: Invalid argument'

            case _:
                return 'Error: Invalid command'

    except ValueError:
        return 'Error: Invalid command format or argument'
    except IndexError:
        return 'Error: LED index out of range'
    except Exception as e:
        return f'Error: {e}'


if __name__ == "__main__":
    main()