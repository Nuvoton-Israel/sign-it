# sign-it
SignIt: a wrapper tool for OpenSSL

# signIt - Binary Signature Generator

 ## Introduction
signIt is a command-line tool which can produce RSA-SHA signature for binary images, and embed the signature in the given file.
	
The tool supports standard unencrypted PKCS#1 RSA key format (PEM, DER)/
	
The tool also support raw binary input of the 256-bytes public modulus (i.e. public key) and 256-bytes private exponent (i.e. private key)
	
 ## Requirements
1) openSSL command-line tool - In order to work with this tool, the OS should support openSSL command-line tool
		-Linux: 
			In many Linux distributions, openSSL command line tool is part of the distribution. this tool was tested on Ubuntu
		-Windows:
			In order to work on Windows station, look for binary openSSL distribution here: https://www.openssl.org/related/binaries.html	
2) compilation tools
		signIt arrives as source code, and should be compiled, according to working station
		-Linux:
			g++ 4.6.3 and up.
		-Windows:
			Microsoft Visual Studio 2013 / Microsoft .NET Framework 4
			
 ## Installation
### Linux:
run make from <signIt dir>/linux_build.
The output can be found at ./deliverables/linux/release/signit
## Windows:
run Clean.bat
run Compile.bat
from ./windows_build
		
 ## Configuration
USAGE:
```
	   signIt.exe  <input file> -p <private_key_file> [-m <public_key_file>]
				   [-f <pem|PEM|der|DER|bin|auto>] [-b <signed_begin_offset>]
				   [-s <signed_area_size>] [-r] [-e <signature_location>] [-o
				   <output_filename>] [--] [--version] [-h]
```

Where:

>   <input file>
	 (required)  Input binary image to sign

>   -p <private_key_file>,  --private_key_file <private_key_file>
	 (required)  Input RSA private key to perform signature with

>   -m <public_key_file>,  --modulu_file <public_key_file>
	 Input RSA public key, (applicable and required only when the key is
	 given in unformatted binary file)

>   -f <pem|PEM|der|DER|bin|auto>,  --keyform <pem|PEM|der|DER|bin|auto>
	 The format in which the input key is supplied. If omitted, key format
	 is identified by file extension. "PEM" and "DER" are standard
	 unencrypted PKCS#1 RSA key formats, "bin" stands for unformatted
	 binary file which contains 256 bytes of the key

>  -b <signed_begin_offset>,  --begin_offset <signed_begin_offset>
	 Offset (in bytes) within the input file from which the signed area
	 starts. If omitted default is 0. If given in hex, prefix with 0x.

>   -s <signed_area_size>,  --signed_size <signed_area_size>
	 Size of signed area (in bytes). If omitted, the signed area assumed to
	 be from the start offset till the end of the file, if given in hex,
	 prefix with 0x

>   -r,  --reverse
	 If this switch is on, the signature will be reversed (big endian to
	 little endian)

>   -e <signature_location>,  --ebmed_signature <signature_location>
	 If this flag is present the output file will be a copy of <input file>
	 and the signature would be embedded inside it at the supplied
	 <signature_location>

>   -o <output_filename>,  --out <output_filename>
	 A path to a binary output file. By default the output file is the
	 signature, if ebmed_signature is selected, the output file is the
	 input image, with the signature embedded inside of it. The default
	 filename is 'signed.bin'

>   --,  --ignore_rest
	 Ignores the rest of the labeled arguments following this flag.

>   --version
	 Displays version information and exits.

>   -h,  --help
	 Displays usage information and exits.



 ## Examples
	
Generating a signature of an entire image using PEM encoded key, to a binary file:
```	
signIt BootBlock.bin -p key_pem.pem -o BB_signature.bin
```	
Generating a signature of a given block in an image using a DER encoded key, and saving the signed image as a new file:
```	
signIt BootBlock.bin -p key_der.der  --begin_offset 0x140 --signed_size 0x1000 --ebmed_signature 0x8 -o BB_signed.bin
```	
Generating a signature a given block in an image using unformatted binary keys, and embedding the reversed signature inside the original file
```	
signIt BootBlock.bin -p private_key.bin -m public_key.bin --begin_offset 0x140 --ebmed_signature 0x8 -o BootBlock.bin -r
```	
Generating a signature a given block in an image using explicit key format, and embedding the signature inside the original file
```	
signIt BootBlock.bin -p key_der.bin --keyform DER -b 0x140 -e 0x8 -o BootBlock.bin
```	

## License 
Based on openSSL which is released under https://www.openssl.org/source/license.html


