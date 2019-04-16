import os


def runCmd(cmd):
    print(cmd)
    result = os.popen(cmd).read()
    print(result)
    return result
    #return os.system(cmd)


def lsCommand(dirName):
    cmd = "ampy -p /dev/ttyUSB0 -b 115200 ls"
    if dirName:
        cmd += " {}".format(dirName)

    result = runCmd(cmd)
    return result
    
def listDirectory(dirName = None):
    result = lsCommand(dirName)
    items = [x.strip("/") for x in result.split()]

    files = []

    for item in items:
        if "." in item:
            # probably file
            files.append(item)
        else:
            files += listDirectory(item)
    return files

def getFile(fileName):
    cmd = "ampy -p /dev/ttyUSB0 -b 115200 get {} {}".format(fileName, fileName)
    return runCmd(cmd)

if __name__ == "__main__":
    
    files = listDirectory()

    print("Files:", files)
    
    for fileName in files:
        getFile(fileName)

