echo "lines words bytes"

wc ./code/*

echo ""

make

if [ -e scedit ]
then
    ./scedit
    rm scedit
else
    echo "compile error"
fi