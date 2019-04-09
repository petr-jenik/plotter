import math

def goto(x, y, e):
    
    if e:
        extrude = "1"
    else:
        extrude = "0"
    cmd = "G0 X{}, Y{}, E{}".format(str(x), str(y), extrude)
    return cmd.decode('utf-8')


def rect(x, y, size):
    print(goto(x - size/2, y - size/2, 1))
    print(goto(x + size/2, y - size/2, 1))
    print(goto(x + size/2, y + size/2, 1))
    print(goto(x - size/2, y + size/2, 1))
    print(goto(x - size/2, y - size/2, 1))


def circle(x, y, r):
    print(goto(x + r, y, 0))
    for i in range(360):
        tmpY = y + r * math.sin(math.radians(i))
        tmpX = x + r * math.cos(math.radians(i))
        print(goto(tmpX, tmpY, 1))
        

for i in range(2, 50, 2):    
    circle(0, 0, i)
    
