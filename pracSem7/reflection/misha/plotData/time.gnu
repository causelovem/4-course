set title "Time of computation for reflection method"
set term png
set style line 1 lt 1 lw 50
set xrange [1:11]
set yrange [0:420]
set xlabel "Number of proc"
set ylabel "Time(sec)"
set xtics ("1" 1, "2" 2, "4" 3, "8" 4, "16" 5, "32" 6, "64" 7, "128" 8, "256" 9, "512" 10, "1024" 11)
set output "time.png"
plot "time.dat" u 1:2 w l title "1024", \
"time.dat" u 1:3 w l title "2048", \
"time.dat" u 1:4 w l title "3072"
