This is a sprite demo, and shows you, how easy it is, to dislplay
a sprite.

Iam using my own sprite Format, this makes the sprite handling
very easy. Every sprite contains the Width and Height, so you
can forget the size of your sprite :)


How to convert a BMP to gp32_sprite format ?

1) Use the bmp2bin tool, to convert the *.bmp to *.raw data.
./bmp2bin -x garfield.bmp garfield.raw
The -x means, that the Width and Height are written to the header.
gp_drawSpriteHT, needs this Format (-x)

2) Convert the *.raw file to a *.c file.
./convert garfield.raw > garfield.c

3) Edit the *.c file and change the name of the [] array.
unsigned char wasteland[] = { 66,7,.....
to
unsigned char garfield[] = { 66,7,......


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Of couse you can use your own sprite converter. ( like the gp one )
! In this case your sprite data does not contain a header with
! the x and y size information. So YOU must know the size of the
! sprite.
! You can use the gp_drawSpriteT or gp_drawSprite function to draw raw 
! sprite data.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


