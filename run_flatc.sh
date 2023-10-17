#!/bin/bash

rm src/jet/IO/Serialization/generated/*

for file in src/jet/IO/Serialization/schema/*.fbs; do
    ./flatc -c -o src/jet/IO/Serialization/generated "$file"
done

read


