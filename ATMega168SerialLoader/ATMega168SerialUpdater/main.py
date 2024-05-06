from serial import Serial

if __name__ == '__main__':
    s = Serial("/dev/cu.SLAB_USBtoUART")
    s.baudrate=9600
    while 1:
        print(s.read())

    print("Ciao!")