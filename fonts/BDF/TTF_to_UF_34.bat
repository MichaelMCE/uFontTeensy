SET size=34

echo off

@call ToBDF_%size%.bat %1

rem echo ###########
rem echo "%1%size%.bdf"
rem echo ###########


bdf2ufont.exe "%1%size%.bdf"