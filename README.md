# Mini_MIPS

## Usage

The project is made to be compiled with GNU make, as follows:

```bash
make
```

This will generate the executable in the ./bin folder. You can also remove the object files and the executable with

```bash
make clean
```

You can use Mini_MIPS as an Instruction Simulator but it can also encode asm into hex files or decode hex to asm.
The following commands are the most useful but you can see all the uses of Mini_MIPS with the --help command

```bash
./bin/Mini_MIPS [options] <input folders>
```

with for example:

```bash
./bin/Mini_MIPS --run    test_file.hex   #launches the simulation of the .hex file
./bin/Mini_MIPS --encode test_file.asm   #launches asm file encoding
./bin/Mini_MIPS --decode test_file.hex   #launches asm file decoding
./bin/Mini_MIPS --step 				     #triggers step mode   ***
./bin/Mini_MIPS --load   test_file.data  #loads data in memory ***
```

Commands marked with *** are not fully functional

*The file `./tests/encode_test_results/test_syracuse_result.hex` is the go-to test file*

## Functionalities

### Assembler
- ASM file conversion with solid blank trimming
- 18 ISS instructions understood
- Label detection and encoding

### Virtual Machine
- Pile creation with hexadecimal instructions
- ISS instructions execution
- Solid command line handling
- Step by step execution with terminal-based GUI (Should be triggered manually by giving step in src/main.c the value 1)

### Cache
- Basic implementation of an 8-lines cache based on a 36-bytes memory
- Writethrough
- Encoding and decoding of adresses and cache lines

### To-do

- Functional cache
- More complete performance tests
- Negative numbers handling

