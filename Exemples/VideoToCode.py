from PIL import Image

file = open('Skyrim.txt', 'w', encoding='cp1251')

for i in range(1, 693):
    s = "Skyrim/00000{0}{1}{2}.bmp".format(i//100, i//10 % 10, i % 10)
    im = Image.open(s)
    pix = im.load()
    for i in range(636):
        for j in range(360):
            file.write("push {0}\n".format(pix[i,j][0] * 256 * 256 + pix[i,j][1] * 256 + pix[i,j][2]))
            file.write("pop [{0}]\n".format(i*360 + j))
    file.write("draw\n")
