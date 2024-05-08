from serial import Serial


def openfile():
    return open("blink.bin", mode="rb")


def serial_open():
    s = Serial("/dev/cu.SLAB_USBtoUART")
    s.baudrate = 9600
    return s


def serial_comm(s):
    while 1:
        sentence = s.readline()
        if b"Boot" in sentence:
            print("Received bootloader string")
            return 1


if __name__ == '__main__':
    s = serial_open()
    serial_comm(s)
    s.write(b"a")
    print(s.read())
    hex_file = []
    f = openfile()
    readed = f.read()

    for i in readed:
        print(hex(i), end=" ")
