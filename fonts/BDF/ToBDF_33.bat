SET size=33

otf2bdf -r 72 -p %size% -o %1%size%.bdf %1
bdf2png %1%size%.bdf
ren %1%size%.bdf.png %1%size%.png


