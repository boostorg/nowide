#!/bin/bash

set -euo pipefail

cd "$(dirname "$0")/.."

if ! [ -e "tools/create_standalone.sh" ]; then
  echo "Could not change to repo root"
  exit 1
fi

targetFolder="${1:-nowide_standalone}"

# If target folder exists fail, unless it is the default in which case it is removed
if [ -e "$targetFolder" ]; then
  if [[ "$targetFolder" == "nowide_standalone" ]]; then
    rm -r "$targetFolder"
  else
    echo "Target folder $targetFolder exists"
    exit 1
  fi
fi

mkdir -p "$targetFolder"/include

cp -r include/boost/nowide "$targetFolder"/include
cp -r src test cmake CMakeLists.txt LICENSE "$targetFolder"
cp standalone/*.hpp "$targetFolder"/include/nowide
mv "$targetFolder/cmake/BoostAddOptions.cmake"  "$targetFolder/cmake/NowideAddOptions.cmake"
mv "$targetFolder/cmake/BoostAddWarnings.cmake" "$targetFolder/cmake/NowideAddWarnings.cmake"
find "$targetFolder" -name 'Jamfile*' -delete

SOURCES=$(find "$targetFolder" -name '*.hpp' -or -name '*.cpp')
SOURCES_NO_BOOST=$(echo "$SOURCES" | grep -v 'filesystem.hpp')

sed 's/BOOST_NOWIDE_/NOWIDE_/g' -i $SOURCES
sed 's/BOOST_/NOWIDE_/g' -i $SOURCES
sed 's/boost::nowide/nowide/g' -i $SOURCES
sed 's/boost::chrono/std::chrono/g' -i $SOURCES
sed 's/boost::milli/std::milli/g' -i $SOURCES
sed 's/boost::/nowide::/g' -i $SOURCES_NO_BOOST
sed '/namespace boost/d' -i $SOURCES
sed 's/<boost\/chrono.hpp/<chrono/g' -i $SOURCES
sed 's/<boost\/nowide\//<nowide\//g' -i $SOURCES
sed 's/<boost\//<nowide\//g' -i $SOURCES_NO_BOOST
sed '/config\/abi_/d' -i $SOURCES

CMLs=$(find "$targetFolder" -name 'CMakeLists.txt' -or -name '*.cmake')

sed 's/ BOOST_ALL_NO_LIB//' -i $CMLs
sed 's/BOOST_NOWIDE_/NOWIDE_/g' -i $CMLs
sed 's/Boost_NOWIDE_/NOWIDE_/g' -i $CMLs
sed 's/boost_nowide/nowide/g' -i $CMLs
sed 's/boost_/nowide_/g' -i $CMLs
sed 's/Boost::nowide/nowide::nowide/g' -i $CMLs
sed 's/Boost/Nowide/g' -i $CMLs
sed 's/ OR BOOST_SUPERPROJECT_SOURCE_DIR//' -i $CMLs

sed '/PUBLIC BOOST_NOWIDE_NO_LIB)/d' -i "$targetFolder/CMakeLists.txt"
sed '/^if(BOOST_SUPERPROJECT_SOURCE_DIR)/,/^endif/d' -i "$targetFolder/CMakeLists.txt"
sed '/add_warnings/atarget_compile_features(nowide PUBLIC cxx_std_11)' -i "$targetFolder/CMakeLists.txt"
sed 's/NAMESPACE Nowide CONFIG_FILE.*$/NAMESPACE nowide)/' -i "$targetFolder/CMakeLists.txt"

sed '/if(NOT BOOST_SUPERPROJECT_SOURCE_DIR)/,/endif/d' -i "$targetFolder/test/CMakeLists.txt"
sed 's/ LIBRARIES Nowide::chrono//' -i "$targetFolder/test/CMakeLists.txt"
sed '/Nowide::filesystem/d' -i "$targetFolder/test/CMakeLists.txt"
