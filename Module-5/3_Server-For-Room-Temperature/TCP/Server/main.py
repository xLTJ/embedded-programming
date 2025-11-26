from socket import *

HOST = "0.0.0.0"
PORT = 8080
BUFFER_SIZE = 1024

def main():
    # setup type shi
    s = socket(AF_INET, SOCK_STREAM)
    s.bind((HOST, PORT))
    s.listen(1)


    print(f"TCP Server listening on port {PORT}...")

    # wait for connections
    while True:
        # accept connection
        conn, addr = s.accept()
        print(f"Connection established from {addr}")

        # receive data
        resp = conn.recv(BUFFER_SIZE)
        print(f"Recieved temperature from {addr}: {resp}")

        conn.close()


if __name__ == "__main__":
    main()