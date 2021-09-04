//james villeneuve fast hex to string byte example for arm64 neon -
//example of mem table based coversion of hex data. when compiled using O3 is highly concurently optimized up to 2 bytes per clock on arm64 neon
// unable to measure 'unrolledloop example' performance at O3 because it eather breaks chronos or it is sub 0 microsecond, O2 is 135microseconds
// g++ O3 hexstringExample hexstringExample.cpp, and g++ -o hexstringExample hexstringExample.cpp 
//run by ./hexstringExample 
//time clock method from https://www.cplusplus.com/reference/chrono/high_resolution_clock/now/ and other sites. is a standard exampl used for timing.
#include <iostream>
//efficient byte decimal to HEX string conversion
//we use a table and let comiler and processor manaage it as global. this way we dont do a lot of copying back and for to stack at each function call. aslo less steps than pointers
static unsigned char hex_upper[257]="0000000000000000111111111111111122222222222222223333333333333333444444444444444455555555555555556666666666666666777777777777777788888888888888889999999999999999aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbccccccccccccccccddddddddddddddddeeeeeeeeeeeeeeeeffffffffffffffff";
static unsigned char hex_lower[257]="0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";
//we keep this glocal to reduce load/unload overhead from stack
char charStore[3]="00";//conatins a terminantor so can be a string as well;
char temp[4];//it is this large for mutiple examples. can be made to size if hex bytes 
//still global because it is faster to read long memory than to load stack with variables at least on arm64

char* convertByteDec(unsigned char byte)
{
//we do not define variables or arrays her as this means time to setup the stack with the data
//and in tight loops this can reduce efficiency. also by using a memory only version we
//do not need to return any values to a set register, so compiler can optimize along with processor
//to do up to two itterations per clock cycle. (if deeply nested)
charStore[0]= hex_upper[byte]; 
charStore[1]= hex_lower[byte];
return charStore;
}
//i don't take most of credit i am just allowing for compiler and cpu to do its magic with some simple clever assistance user takes on mem cache responsability while system
//no longer copies variabel data to stack in each function call. manual unroll is an obvious system syncronous time saver as it allows pipeline optimizations of a specific area
//compiler seems to treat it differently than fun-unroll-all-loops?
//may need spell check and is draft version as of sept 3, 2021 ther also will be a array of hex to string converter posted soon, hopefully.... :)
//GNU lisence and ony if above this line is copied. leave name at top and copy all lines above. 


//below here is example code
/*
 * with standard -o
 * Time taken by function: 2290 microseconds
 * with itterations of 65536
 * 
 * with O2 settings
 * Time taken by function: 135 microseconds
 * with itterations of 65536
 * 
 * with O3 settings optimizations unsure if chronos breaks
 * Time taken by function: 0 microseconds
 * with itterations of 65536
* an operations cycle is completed in 0.000000087600708s
 * /
 */ 
//example 4
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;
//example code
int main(int argc, char* argv[])
{
//example1	outputing result using cout or printf
int i=0;
while (i<256){
printf(convertByteDec(i));
printf("; ");
i++;}
printf("\n");
//example two directly copying from hex values stored for byte
convertByteDec(0x80);//in decimal would be 128
temp[0] = charStore[0];
temp[1] = charStore[1];

printf(temp);//we prin var with stored chars
printf("as output from hex");
printf("\n");


//example three directly copying from hex values stored for byte
uint16_t large_16bitNumber=0x441f ;// use any number example
uint8_t highbyte=large_16bitNumber>>8;//yes there is math used here
uint8_t lowbyte=large_16bitNumber &255;//but not all these are used every 
// time and usually only 1 byte would change in data stream at a time.
// also the math can be done with severl concurent mem calls
//  so it is still faster than using math to change byte data
convertByteDec(highbyte);

temp[0] = charStore[0];
temp[1] = charStore[1];
convertByteDec(lowbyte);
temp[2] = charStore[0];
temp[3] = charStore[1];
printf("%u", large_16bitNumber);
printf(" as a string: ");
printf(temp);//we print var with stored chars
printf(" as a string \n");

//4th example timing conversion rate
printf(" will now do 65536 cycles of 1 byte hex conversions \n");
auto start = high_resolution_clock::now();//we start timing the example
    // Get starting timepoint
    long counter=0;//prev declared
    int j=0;//prev declared
    do{
	int i=0;
	do{
			convertByteDec(i);//we get ram table values of hex
			temp[0] = charStore[0];//we get stored results
			temp[1] = charStore[1];//and place them into a var so compilers doesn't optimize out
			counter++;
			i++;
		}while(i<256);
		j++;
	}while(j<256);
	
     auto stop = high_resolution_clock::now();
  
    // Get duration. Substart timepoints to 
    // get durarion. To cast it to proper unit
    // use duration cast method
    auto duration = duration_cast<microseconds>(stop - start);
  
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
         cout << " with itterations of " << counter;

         printf("\n");   
        
         printf(" will now do 65536 cycles of 1 byte hex conversions  with 1 level of full unrolls \n");
 auto start2 = high_resolution_clock::now();//we start timing the example
    // Get starting timepoint
 counter=0;//prev declared
 j=0;//prev declared
    do{
		
//we setup for a manual crazy unroll	
int i;	//we need to declare before manual unroll
#define unroll1	 i=0;
//#define unroll2           do{
// on defined lines no comments because it will break combined commands during manual unroll
//we get ram table values of hex
#define unroll3			convertByteDec(i);
//we get stored results
#define unroll4			temp[0] = charStore[0];
//and place them into a var so compilers doesn't optimize out
#define unroll5			temp[1] = charStore[1];
			
#define unroll6			i++;
//#define unroll7		}while(i<256);
//single line for all commands to change a byte to one line as group_
#define group_ unroll1 unroll3 unroll4 unroll5 unroll6 
//combine to 4*8=32 unrools
#define combineto8a group_ group_ group_ group_ group_ group_ group_ group_
#define combineto8b group_ group_ group_ group_ group_ group_ group_ group_
#define combineto8c group_ group_ group_ group_ group_ group_ group_ group_
#define combineto8d group_ group_ group_ group_ group_ group_ group_ group_
#define allgroups combineto8a combineto8b combineto8c combineto8d

// combine 32 *8=256 unrolls with each unroll having an i++;
#define crazy_manual_unroll allgroups allgroups allgroups allgroups allgroups allgroups allgroups allgroups
		
		crazy_manual_unroll;//execute manual unoll!
		counter+=256;//we run unroll all at once
		j++;
	}while(j<256);
	
  auto    stop2 = high_resolution_clock::now();
  
    // Get duration. Substart timepoints to 
    // get durarion. To cast it to proper unit
    // use duration cast method
     duration = duration_cast<microseconds>(stop2 - start2);
  
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
         cout << " with itterations of " << counter;

         printf("\n");   
         
        
return 0;
}

//group of char with an unknown string size, it is up to creator to ensure string terminates
