set title "Sequence research"
set xlabel "d_x_y_z"
set ylabel "Sequences difference"
set grid
set style data points
plot "research_data.txt" using 1:2 with linespoints title ""
pause -1