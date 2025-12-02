from multiprocessing import shared_memory as sm
from time import sleep

def main():
    commandShm = sm.SharedMemory(name="commands", create=False, size=16)
    reponseShm = sm.SharedMemory(name="responses", create=False, size=64)

    while True:
        try:
            user_input = input("> ").encode()
            commandShm.buf[:] = b'\x00' * len(commandShm.buf) # reset buffer
            commandShm.buf[0] = 1 # set first byte to 1 to show a new command is available
            commandShm.buf[1:1+len(user_input)] = user_input

            # wait for response
            while reponseShm.buf[0] == 0:
                sleep(0.1)

            # print response
            data = reponseShm.buf[1:].tobytes()
            response = bytes(data).decode('utf-8').strip('\x00')
            print(f"response: {response}")

            reponseShm.buf[0] = 0
        except KeyboardInterrupt:
            commandShm.close()
            break

if __name__ == "__main__":
    main()