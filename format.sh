#!/bin/sh

src=$(find ./ -name "*.cpp")
src="$src $(find ./ -name "*.h")"
src="$src $(find ./ -name "*.CPP")"
src="$src $(find ./ -name "*.H")"
#echo "$src" > /tmp/__temp_format_sh.txt
#for f in ; do echo "$src f"; done
echo "$src" | while IFS= read -r src; do
   uncrustify -c uncrustify.cfg --no-backup --replace $src
done

#uncrustify -c uncrustify.cfg --no-backup --replace $src
