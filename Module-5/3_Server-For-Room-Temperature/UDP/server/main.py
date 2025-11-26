from socket import *

HOST = "0.0.0.0"
PORT = 8080
BUFFER_SIZE = 1024

def main():
    # setup type shi
    s = socket(AF_INET, SOCK_DGRAM)
    s.bind((HOST, PORT))
    s.listen(1)


    print(f"UDP Server listening on port {PORT}...")

    # receive datagrams
    while True:
        resp, addr = s.recvfrom(BUFFER_SIZE)
        print(f"received temperature from {addr}: {resp}")


if __name__ == "__main__":
    main()