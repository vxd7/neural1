#!/bin/bash

if [ -s "doc.zip" ]; then
	rm doc.zip
fi

zip -j doc.zip doxydocs/html/*
