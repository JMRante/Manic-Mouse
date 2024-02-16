# Manic Mouse

### Purpose

Manic Mouse is a small arcade game where you move a mouse around with the... mouse. 
It is written in C++ and made for 64-bit Windows. This game is the next step after
MinSnake in terms of complexity, focusing on using SDL2 for a more full experience
using external asset files for images and audio. The intent was to build code for
the game itself, not trying to create a generalized engine system. However, a lot of
the code and architecture can be reused in later games.

There are 50 levels to play, designed in the Tiled editor tool and then transformed via
a Node.js script to a format the game can read. The end result I think is a fun design,
and I was happy with the level variety I squeezed out given the incredibly limited selection
of gameplay elements.

### Compile

Open the VS project in the repository in Visual Studio C++. Then build the project in the
standard way that C++ projects are compiled in Visual Studio C++.

### Dependencies

Manic Mouse utilizes the C++ standard library, SDL2, and GLEW.

### Audio Source Credits

669338__geoff-bremner-audio__keys-grab-hard-2 by Geoff-Bremner-Audio

537828__postproddog__open-door-knob-step-in-close-door by PostProdDog

472399__joseagudelo__16_raton_chillando by JoseAgudelo

411597__omgbong__eating-an-apple-01 by omgbong

78562__joedeshon__alarm_clock_ringing_01 by joedeshon

Sneaky Feet by geoffharvey