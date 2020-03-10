from PIL import Image, ImageDraw, ImageFont
import cv2
import numpy as np
def drawText(img,txt,font,pos,oritation = "h",color = "#00ff00"):
    size = font.size
    image = img.copy()
    if oritation == "h":
        draw = ImageDraw.Draw(image)
        draw.text(pos, txt, font=font, fill=color)
    # for i in range(len(text.content)):
    #     draw.text((pos[0]+i*size,pos[1]),text.content[i],font = text.font,fill = color)
    elif oritation == "v":
        lines = txt.split("\n")
        for i in range(len(lines)):
            # tempText = text
            # tempText.setText(lines[i])
            image = drawOneLineAlongVertical(image,lines[i],font,(pos[0]+i*size,pos[1]),color)
    return image


def drawOneLineAlongVertical(img,txt,font,pos,color = "#00ff00"):
    image = img.copy()
    size = font.size
    assert(not "\n" in txt),"must be only one line"
    draw = ImageDraw.Draw(image)
    for i in range(len(txt)):
        draw.text((pos[0],pos[1]+i*size),txt[i],font =font,fill = color)
    return image

def parse_arguments():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--font_path',type=str,help="font file path")
    parser.add_argument('--img_size',type=str,nargs='+', help='format h x w')
    parser.add_argument('--font_size',type=int,help='set font size')
    parser.add_argument('--txt',type=str,help='txt')
    parser.add_argument('--orientation',choices=['v','h'],help='set orientation, v means vertical, h means horizontal')
    parser.add_argument('--offset',nargs='+',type=int,help='start positon')
    parser.add_argument('--color')
    parser.add_argument('--output', help='output image path')
    parser.add_argument('--mask', default=None, help='output image mask path')
    return parser.parse_args()

if __name__ == '__main__':
    args = parse_arguments()
    fontSize = args.font_size
    fontFilePath=args.font_path

    font = ImageFont.truetype(fontFilePath, fontSize)
    txt=args.txt
    size_arg = ''
    for s in args.img_size:
        size_arg += s
    hw = size_arg.split('x')
    h = int(hw[0])
    w = int(hw[1])

    pos_x, pos_y = args.offset

    image = Image.new('RGB', (w,h))
    image2 = drawText(image,txt,font,(pos_x,pos_y),args.oritation,color=args.color)
    image2.save(args.output)
    if args.mask is not None:
        image2_mask = drawText(image,txt,font,(pos_x,pos_y),args.oritation,color="#ffffff")
        
        image_mask = np.array(image2_mask)
        kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(2,3))
        dst = cv2.erode(image_mask,kernel)
        cv2.imwrite(args.mask,dst);
    # image2.show()