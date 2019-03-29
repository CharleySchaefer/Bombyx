#-----------------------------------------------------------------------------
#    Bombyx: Modelling of linear viscoelasticity
#    Copyright (C) 2019 Charley Schaefer <The University of York, UK>

#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#-----------------------------------------------------------------------------
#!/bin/bash

version=`awk <src/main.h '$1=="#define" && $2=="BBX_VERSION" {print $3}'`
echo " "
echo "Bombyx $version Copyright (C) 2019 Charley Schaefer"
echo "This program comes with ABSOLUTELY NO WARRANTY; "
echo "for details type \"$0 --copying\"."
echo "This is free software, and you are welcome to redistribute it"
echo "under certain conditions; type \"$0 --copying\" for details."
echo " "

if [ $# -gt 0 ]; then
  if [ $1 == "--copying" ]; then
    cat LICENSE.txt
    exit 1
  fi
fi

echo "Creating build directory."
mkdir -p build

echo "Copying utils/runBombyx.sh and utils/FitReport.html to build"
cp utils/runBombyx.sh   build
cp utils/FitReport.html build

pushd build > /dev/null
  ./runBombyx.sh -c
popd > /dev/null

echo " "
echo "Bombyx $version Copyright (C) 2019 Charley Schaefer"
echo "This program comes with ABSOLUTELY NO WARRANTY; "
echo "for details type \"$0 --copying\"."
echo "This is free software, and you are welcome to redistribute it"
echo "under certain conditions; type \"$0 --copying\" for details."
echo " "
