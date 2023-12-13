import os
import tkinter

# import pygame
# pygame.mixer.init()
items = []  # 初始化选项列表

root = tkinter.Tk()  # 创建窗口
root.title('GBA游戏模拟器')  # 设置标题
root.geometry('400x600')  # 设置窗口大小为500x300
root.resizable(width=False, height=False)  # 禁止修改窗口大小

x1 = 90

img_gif = tkinter.PhotoImage(file='temp.gif')
label_img = tkinter.Label(root, image=img_gif)
label_img.place(x=120, y=15)

file = r'bgm.mp3'


# pygame.mixer.music.load(file)
# pygame.mixer.music.play()


def choose_a():
    os.startfile('.\PONG\PONG.exe')


def choose_b():
    os.startfile('.\Snake\Snake.exe')


def choose_c():
    os.startfile('.\AceCombat\AceCombat.exe')


def choose_d():
    os.startfile('1.exe')


GA = tkinter.Button(root, text='PONG', command=choose_a)
GA['width'] = 30
GA['height'] = 3
GA.place(x=x1, y=200)

GB = tkinter.Button(root, text='Snake', command=choose_b)
GB['width'] = 30
GB['height'] = 3
GB.place(x=x1, y=300)

GC = tkinter.Button(root, text='Ace Combat', command=choose_c)
GC['width'] = 30
GC['height'] = 3
GC.place(x=x1, y=400)

GD = tkinter.Button(root, text='Flappy Bird', command=choose_d)
GD['width'] = 30
GD['height'] = 3
GD.place(x=x1, y=500)

root.mainloop()
