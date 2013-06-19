
Sally - A Tool for Embedding Strings in Vector Spaces
==

Introduction
-- 

Sally is a small tool for mapping a set of strings to a set of vectors. 
This mapping is referred to as embedding and allows for applying
techniques of machine learning and data mining for analysis of string
data.  Sally can be applied to several types of strings, such as text
documents, DNA sequences or log files, where it can handle common formats
such as directories, archives and text files of string data.

Sally implements a standard technique for mapping strings to a vector
space that is often referred to as vector space model or bag-of-words
model.  The strings are characterized by a set of features, where each
feature is associated with one dimension of the vector space.  The
following types of features are supported by Sally: bytes, words, n-grams
of bytes and n-grams of words.

Sally proceeds by counting the occurrences of the specified features in
each string and generating a sparse vector of count values. 
Alternatively, binary or TF-IDF values can be computed and stored in the
vectors.  Sally then normalizes the vector, for example using the L1 or L2
norm, and outputs it in a specified format, such as plain text or in
LibSVM or Matlab format.

Consult the manual page of Sally for more information.

Dependencies
--

    >= zlib-1.2.1
    >= libconfig-1.4
    >= libarchive-2.70

Corresponding packages

+ Debian and Ubuntu Linux: `libz-dev libconfig9-dev libarchive-dev`  
+ MacOS X with Homebrew:   `libconfig libarchive (homebrew-dupes)`  
+ MacOS X with MacPorts:   `libconfig-hr libarchive`  

The source code of the libraries is available here:

+ zlib        <http://www.zlib.net/>
+ libconfig   <http://www.hyperrealm.com/libconfig/>
+ libarchive  <http://libarchive.github.com/>

Compilation & Installation
--

From GIT repository first run

    $ ./bootstrap

From tarball run

    $ ./configure [options]
    $ make
    $ make check
    $ make install

Options for configure

    --prefix=PATH           Set directory prefix for installation

By default Sally is installed into /usr/local. If you prefer a
different location, use this option to select an installation
directory.

    --enable-libarchive     Enable support for loading archives
     
If this feature is enabled, Sally can also be applied to read the
contents of archives, such as .tgz and .zip.  This allows for
processing string data in compressed form and may drastically save
storage space.

    --enable-openmp         Enable support for OpenMP (experimental)
    
This feature enables support for OpenMP in Sally. It is still
experimental.  Sally will execute certain parts of the processing in
parallel making use of multi-core architectures where possible.
 
    --enable-md5hash        Enable MD5 as alternative hash

Sally uses a hash function for mapping different features to
different dimensions in the vector space.  By default the very
efficient Murmur hash is used for this task.  In certain critical
cases it may be useful to use a cryptographic hash as MD5.

Copyright (C) 2010-2013 Konrad Rieck (konrad@mlsec.org);
			Christian Wressnegger (christian@mlsec.org);
			Alexander Bikadorov (abiku@cs.tu-berlin.de)
