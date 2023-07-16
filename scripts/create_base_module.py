MODULE_DIRECTORY = "../modules/"

def main():
    createFile("test.cpp", "message\ntest\n")

def createFile(fileName, contents):

    file = open(MODULE_DIRECTORY + fileName, "w")

    file.write(contents)

    file.close()

if __name__ == "__main__":
    main()