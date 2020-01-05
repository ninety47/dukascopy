lib-dukascopy
=============
This is a C++ library for extracting tick data from the raw and compressed
binary data files the you can download from Dukascopy.

License
=======
lib-dukascopy is released under the GNU General Public License (GPL) v3. A
copy of the license is available in license.txt


Disclaimer
==========
The data files that this library processes are provided by Dukascopy. I make
no guarantee about the quality of the data they provide, and the reliability 
of this library to extract the data from these files.


File format
===========
The files I downloaded are named something like '00h_ticks.bi5'. These 'bi5'
files are LZMA compressed binary data files. The binary data file are formatted
into 20-byte rows. 

* 32-bit integer: milliseconds since epoch
* 32-bit float: Ask price
* 32-bit float: Bid price
* 32-bit float: Ask volume
* 32-bit float: Bid volume

The *ask* and *bid* prices need to be multiplied by the point value for the 
symbol/currency pair.

The *epoch* is extracted from the URL (and the folder structure I've used to 
store the files on disk). It represents the point in time that the file starts
from e.g. 2013/01/14/00h_ticks.bi5 has the epoch of midnight on 14 January 2013.

Example
=======

In order to run the test examples you first need to build the project

    mkdir build/
    cd build/
    cmake ..
    make


To uncompress a `.bi5`, simplest is to run the test example `test_read_bi5` from `build/`

    LD_LIBRARY_PATH=./ninety47-dukascopy-0.1.0/lib/ ./test/test_read_bi5






