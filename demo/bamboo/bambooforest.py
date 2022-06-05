import sys
import random

total = 100
segmin = 5
segmax = 15
minskew = 0
maxskew = 20
minscale = 0.8
maxscale = 1.5

#Cor do bamboo: 155, 255, 0
#Cor da relva: 80, 207, 40
#Cor dos aneis: 150, 200, 40

bamboofile = open("bamboo.xml", "w+")
bamboofile.write("<world>\n\t<camera>\n\t\t<position x=\"50\" y=\"10\" z=\"50\" />\n\t\t<lookAt x=\"55\" y=\"10\" z=\"55\" />\n\t\t<up x=\"0\" y=\"1\" z=\"0\" />\n\t\t<projection fov=\"60\" near=\"1\" far=\"1000\" />\n\t</camera>\n")
bamboofile.write("\t<group>\n\t\t<transform>\n\t\t\t<translate x=\"50\" y=\"0\" z=\"50\" />\n\t\t</transform>\n\t\t<models>\n\t\t\t<model file=\"floor.3d\">\n\t\t\t\t<color>\n\t\t\t\t\t<diffuse R=\"0\" G=\"0\" B=\"0\" />\n")
bamboofile.write("\t\t\t\t\t<ambient R=\"100\" G=\"220\" B=\"40\" />\n\t\t\t\t\t<specular R=\"0\" G=\"0\" B=\"0\" />\n\t\t\t\t\t<emissive R=\"30\" G=\"100\" B=\"10\" />\n\t\t\t\t\t<shininess value=\"128\" />\n\t\t\t\t</color>\n\t\t\t</model>\n\t\t</models>\n\t</group>\n")
def writeIndent(indentCount):
    for i in range (indentCount):
        bamboofile.write("\t")

def writeBamboo(segs, indent, angle, axisx, axisz):
    writeIndent(indent)
    bamboofile.write("<group>\n")
    writeIndent(indent + 1)
    bamboofile.write("<transform>\n")
    writeIndent(indent + 2)
    bamboofile.write("<rotate angle=\"" + str(angle) + "\" x=\"" + str(axisx) + "\" y=\"0\" z=\"" + str(axisz) + "\" />\n")
    writeIndent(indent + 2)
    bamboofile.write("<scale x=\"0.95\" y=\"0.95\" z=\"0.95\" />\n")
    writeIndent(indent + 2)
    bamboofile.write("<translate x=\"0\" y=\"3.8\" z=\"0\" />\n")
    writeIndent(indent + 1)
    bamboofile.write("</transform>\n")
    writeIndent(indent + 1)
    bamboofile.write("<models>\n")
    writeIndent(indent + 2)
    bamboofile.write("<model file=\"bambooring.3d\">\n")
    writeIndent(indent + 3)
    bamboofile.write("<color>")
    writeIndent(indent + 4)
    bamboofile.write("<diffuse R=\"150\" G=\"200\" B=\"40\" />")
    writeIndent(indent + 4)
    bamboofile.write("<ambient R=\"75\" G=\"200\" B=\"90\" />")
    writeIndent(indent + 4)
    bamboofile.write("<specular R=\"75\" G=\"100\" B=\"75\" />")
    writeIndent(indent + 4)
    bamboofile.write("<emissive R=\"0\" G=\"0\" B=\"0\" />")
    writeIndent(indent + 4)
    bamboofile.write("<shininess value=\"128\" />")
    writeIndent(indent + 3)
    bamboofile.write("</color>")
    writeIndent(indent + 2)
    bamboofile.write("</model>")
    writeIndent(indent + 2)
    bamboofile.write("<model file =\"bambooslice.3d\">\n")
    writeIndent(indent + 3)
    bamboofile.write("<color>")
    writeIndent(indent + 4)
    bamboofile.write("<diffuse R=\"175\" G=\"255\" B=\"0\" />")
    writeIndent(indent + 4)
    bamboofile.write("<ambient R=\"190\" G=\"255\" B=\"10\" />")
    writeIndent(indent + 4)
    bamboofile.write("<specular R=\"75\" G=\"100\" B=\"75\" />")
    writeIndent(indent + 4)
    bamboofile.write("<emissive R=\"0\" G=\"0\" B=\"0\" />")
    writeIndent(indent + 4)
    bamboofile.write("<shininess value=\"128\" />")
    writeIndent(indent + 3)
    bamboofile.write("</color>")
    writeIndent(indent + 2)
    bamboofile.write("</model>")
    writeIndent(indent + 1)
    bamboofile.write("</models>\n")
    
    if (segs > 1):
        writeBamboo(segs - 1, indent + 1, angle, axisx, axisz)
        
    writeIndent(indent)
    bamboofile.write("</group>\n")


positionsdict = { }
for i in range(total):
    positionx = (float)(random.randint(0, 1000)) / 10
    positionz = (float)(random.randint(0, 1000)) / 10
    while (positionx, positionz) in positionsdict.keys():
        positionx = (float)(random.randint(0, 1000)) / 10
        positionz = (float)(random.randint(0, 1000)) / 10
    positionsdict[(positionx, positionz)] = True

    segs = random.randint(segmin, segmax)

    angle = (float)(random.randint(minskew, maxskew * 10 * 2)) / 10 - maxskew
    angleDif = angle / segs
    rotationx = (float)(random.randint(0, 10)) / 10
    rotationz = (float)(random.randint(0, 10)) / 10

    scalex = (float)(random.randint(minscale * 10, maxscale * 10)) / 10
    scaley = (float)(random.randint(minscale * 10, maxscale * 10)) / 10
    scalez = (float)(random.randint(minscale * 10, maxscale * 10)) / 10


    bamboofile.write("\t<group>\n")
    bamboofile.write("\t\t<transform>\n")
    bamboofile.write("\t\t\t<translate x=\"" + str(positionx) + "\" y=\"-3\" z=\"" + str(positionz) + "\" />\n")
    bamboofile.write("\t\t\t<scale x=\"" + str(scalex) + "\" y=\"" + str(scaley) + "\" z=\"" + str(scalez) + "\" />\n")
    bamboofile.write("\t\t</transform>\n")

    writeBamboo(segs, 2, angleDif, rotationx, rotationz)
    bamboofile.write("\t</group>\n")

bamboofile.write("</world>")
bamboofile.close()