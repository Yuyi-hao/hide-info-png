# Reference 
- PNG specification - [Link to read more about PNG formate](http://www.libpng.org/pub/png/spec/1.2/PNG-Contents.html

# Notes:
>  Not to forget in C `""[Double quote]` is used for string and `''[single quote]` is used for character.
- We are using `argv` instead of `argc` coz it always end with `NULL` and its easy to find where is the end of the arguments. 
- `argv` work as stack so every time we do `agrv++` we are popping out the topmost argument or can say next argument 
- To move to next argument  just increment `*next_arg = **argv++`.
- First argument is always the program. 
- `assert(*argv != NULL);`  // Checks that at least program name is provided if it is not NULL then execute following statement otherwise no.
- - Read file in byte format `fread(result store in , size of single element we try to read, the amount of each element you want to read, file pointer)`  we can switch also between size of the element and number of the element 
    - fread return an integer if everything works fine otherwise it can have two situation
        1. reached end of file it will set `feof` to true 
        2. if it can't read file for any reason it will set `ferror` flag
- To print the buffer use `%u` and all other same as printing array.
- In C you can assign any pointer type to any pointer type if you r not having a warning flag ig _need to check it again_ but then you can assign any pointer type to void pointer type and then  void pointer type to any pointer type thats why we have void pointer type in `read_buff` function.
- 
- memcmp -> **left to read**
- type punning -> **left to read**
- ploglot program -> **left to read**
- we can not pass arrays by value in C. [didn't understand fully]




# About PNG
- PNG file consist of signature followed by a series of chunks.
- The first eight byte of a png file always contain the following(decimal) value:
```
    137 80 78 71 13 10 26 10
```
- The signature indicate that the remainder contain a single PNG image, consist of series of chucks begin with **IHDR** and end with **IEND** chucks.
- **One more point to write about chunk layout**
- The chunk layout is divided into four parts named _size, type, data, CRC(for checksum)_ and each of them is four bytes 
    - Size gives the size of only `data` field it doesn't include `type` and `CRC`.
- 

# File handling
- File is storage of data on disk so that it can be retrieve later and use as we want basically file is use to store data permanently as main memory is volatile.
- All the data stored on the disk is in binary format and how this binary data is stored on disk is depends on OS. 
```
    ---------------     ----------------------     ------
    | Our program | --> | C Library function | --> | OS | --> Disk
    ---------------     ----------------------     ------
```
## Basic operation on  file 
1etrt. Opening an existing file 
2. Reading from a file 
3. Writing to a file 
4. creating a new file 
5. Moving a file(seeking)
6. Closing a file 

```c
// Example:
int main(){
    FILE *f = fopen('file_path', "Mode(read[r], write[w], read binary[rb], write binary[wb]), append[a");  // file pointer 
    char ch;
    while(1){
        ch = fgetc(fp); 
        if(ch = EOF){
            // EOF -> End Of File
            break; 
        }
        printf("%c", ch);
    }
    printf("\n");
    fclose(fp); // Close the file to prevent memory consumption
    
} 
```

