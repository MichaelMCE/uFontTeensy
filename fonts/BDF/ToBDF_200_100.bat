SET size=200

otf2bdf -r 100 -p %size% -o %1%size%_100.bdf %1
bdf2png %1%size%_100.bdf
ren %1%size%_100.bdf.png %1%size%_100.png