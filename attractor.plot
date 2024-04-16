set title "Lorenz"
set xlabel "x"
set ylabel "y"
set zlabel "z"
set parametric
set urange [-32.5:5]
set vrange [-10:27.5]
splot -7.18, v+u, v-u title "A",\
7.15, v+u, v-u title "B",\
"attractor_data.txt" using 1:2:3 with lines  title "Lorenz attractor"

pause -1
