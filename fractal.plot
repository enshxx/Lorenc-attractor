set terminal png size 4000, 4000
set output "fragment.png"

set palette rgbformulae 33,13,10
set palette maxcolors 256

unset key
unset border
unset colorbox
unset xtics
unset ytics
set logscale cb
set size ratio -1
plot "fractal.txt" using 1:2:3 with image pixels
