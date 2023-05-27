#!/bin/zsh
mkdir -p tmp/melcrjos
cp -r src tmp/melcrjos/
cp -r examples tmp/melcrjos/
cp -r libs tmp/melcrjos/
cp zadani.txt tmp/melcrjos/
cp prohlaseni.txt tmp/melcrjos/
cp Doxyfile tmp/melcrjos/
cp Makefile tmp/melcrjos/
cp README.md tmp/melcrjos/
cd tmp
zip -r ../melcrjos.zip melcrjos
cd ..
rm -rf tmp
