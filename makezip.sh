#!/bin/zsh

LOGIN=melcrjos

mkdir -p tmp/$LOGIN
cp -r src tmp/$LOGIN
cp -r examples tmp/$LOGIN/
cp -r libs tmp/$LOGIN/
cp zadani.txt tmp/$LOGIN/
cp prohlaseni.txt tmp/$LOGIN/
cp Doxyfile tmp/$LOGIN/
cp Makefile tmp/$LOGIN/
cp README.md tmp/$LOGIN/
cd tmp
zip -r ../$LOGIN.zip $LOGIN
cd ..
rm -rf tmp
