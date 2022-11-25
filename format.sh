#!/bin/sh

src=$(find ./ -name "*.cpp")
src="$src $(find ./ -name "*.h")"
src="$src $(find ./ -name "*.CPP")"
src="$src $(find ./ -name "*.H")"
echo "$src" | while IFS= read -r src; do
   uncrustify -c uncrustify.cfg --no-backup --replace $src
done
