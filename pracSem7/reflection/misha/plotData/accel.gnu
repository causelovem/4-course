set title "Acceleration for reflection method"
set term png
set style line 1 lt 1 lw 50
set xrange [1:10]
set yrange [0:400]
set xlabel "Number of proc"
set ylabel "Acceleration"
set xtics ("2" 1, "4" 2, "8" 3, "16" 4, "32" 5, "64" 6, "128" 7, "256" 8, "512" 9, "1024" 10)
set output "accel.png"
plot "accel.dat" u 1:2 w l title "1024", \
"accel.dat" u 1:3 w l title "2048", \
"accel.dat" u 1:4 w l title "3072"
