#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define INFO_NUMBERS 6				//有6个输出信息
#define BUFFERMAX 30				// how many buffer size of read filearguments

// defination of the data_structure 
typedef struct Cache{
	struct Set** sets;				// Cache contains a Set array, array stores the address if set
} Cache;

typedef struct Set {
	struct Line** lines;				// Set contains a Line array, array store the address of line
} Set;

typedef struct Line {
	int v;							// valid bit
	int tag;						// tag
	int usage_times;				// record the times of the used block
} Line;

// defination of help infor mation
const char *info[INFO_NUMBERS] = {
"-h: Optional help flag that prints usage info",
"-v: Optional verbose flag that displays trace info",
"-s <s>: Number of set index bits(S = 2^s is the number of sets)",
"-E <E>: Associativity (number of lines per set)",
"-b <b>: Number of block bits(B = 2^b is the block size)",
"-t <tracefile>: Name of the valgrind trace to repay"
};

int hits = 0;
int misses = 0;
int evictions = 0;

// function definations
/* Cache init: according to the argumrnts set_numbers, line_numbers,
 * set_numbers: how many sets in the cache
 * line_numbers: how many lines in a set
 */
Cache *CacheInit(int set_numbers, int line_numbers)
{
	Cache *cache = malloc(sizeof(Set)*set_numbers);
	Set **sets_address = malloc(sizeof(Set*)*set_numbers);
	cache->sets = sets_address;

	/* first loop init each set in the cache
     * second loop init each line in the set
	 */
	for(int i = 0; i < set_numbers; i++) {
		Set *set = malloc(sizeof(Set));
		Line **lines_address = malloc(sizeof(Line*)*line_numbers);
		set->lines = lines_address;
		for(int j = 0; j < line_numbers; j++) {
			Line *line = malloc(sizeof(Line));
			line->v = 0;
			line->tag = 0;
			line->usage_times = 0;
			printf("111test\n");
			set->lines[j] = line;
		}
		cache->sets[i] = set;
	}
	return cache;
}

// Use for test the CacheInit function
void CacheInit_test(Cache *cache, int set_numbers, int line_numbers)
{
	for(int i = 0; i < set_numbers; i++) {
		printf("This is %d set data\n\t", i);
		Set *set_address = cache->sets[i];
		for(int j = 0; j < line_numbers; j++) {
			Line *lines_address = set_address->lines[j];
			printf("This is the %d line data: ", j);
			printf("v=%d, tag=%d, usage_times=%d\n", lines_address->v, lines_address->tag, lines_address->usage_times);
		}
	}
}

// Use for print the help info
void PrintInfo()
{
	for(int i=0; i<INFO_NUMBERS; i++) {
		printf("%s\n", info[i]);
	}
}

// Use for calculate the numbers power
int Power(int base, int times) {
	int result = 1;
	for(int i=0; i<times; i++) {
		result = result * base;
	}
	return result;
}	

// transfor the hex string to decimal
int Hex2Decimal(char *hex) {
	int length = strlen(hex);
	int ch = 0;
	int base = 0;
	int result = 0;
	for(int i=0; i<length; i++) {
		ch = hex[i];
		if(isdigit(ch)) {
			base = ch - 48;
		}
		else {
			base = ch - 87;
		}
		result += base * Power(16, length-(i+1));
	}
	
	return result;
}

// Cache match use for implement simulator of cache map
void CacheMatch(Cache *cache, int set_address, int target)
{
	misses++;
	hits++;
}

// Cache match use for implement simulation of cache map for option v
void VCacheMatch(Cache *cachem, int set_address, int target)
{
	hits++;
	evictions++;
}
				

int main(int argc, char *argv[])
{
	// defination of the use variables
	int set_numbers = 0;						// record how many sets
	int set_bits = 0;							// record how many bits to represent the number of set
	int line_numbers = 0;						// record how many lines
	int block_bits = 0;							// record how many bits to represent the size of block
	char *file_path = NULL;					// record the path of file
	FILE *fp = NULL;
	Cache *cache = NULL;

	// analyse the arguments
	int ch;
	int exist_h = 0;
	int exist_v = 0;
	while((ch = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
		switch(ch) {
			case 'h':
				PrintInfo();
				exist_h = 1;									// ensure only input h is correct
				break;
			case 'v':
				exist_v = 1;									// use for record whether output the message
				break;
			case 's':
				set_bits = atoi(optarg);
				set_numbers = Power(2, set_bits);
				printf("this is a test: %d\n", set_numbers);
				break;
			case 'E':
				line_numbers = atoi(optarg);
				printf("this is second test: %d\n", line_numbers);
				break;
			case 'b':
				block_bits = atoi(optarg);
				printf("this is third test: %d\n", block_bits);
				break;
			case 't':
				file_path = optarg;
				printf("this is the file path: %s\n", file_path);
				break;
			default:
				printf("Error: no such option, please input -h for more information!\n");
				return 1;
				break;
		}
	}
	
	// judge correctness of arguments
	if (set_numbers == 0 || line_numbers == 0 || block_bits == 0) {
		if (exist_h) return 0;
		printf("Please input correct numbers\n");
		return 2;
	} 
	cache = CacheInit(set_numbers, line_numbers);
	CacheInit_test(cache, set_numbers, line_numbers);
	
	// address the file
	fp = fopen(file_path, "r");
	if(fp == NULL) {
		printf("Error: please check your file address!\n");
		return 3;
	}

	// analyse every lines of the file
	char arguments[BUFFERMAX]; 
	char *op;
	char *memory_address_hex;
	int memory_address;
	int set_address;
	int target;
	while(fgets(arguments,BUFFERMAX,fp)) {
		op = strtok(arguments, " ");
		memory_address_hex = strtok(NULL, ",");
		memory_address = Hex2Decimal(memory_address_hex);
		// use bits of memory_address to match cache
		set_address = (memory_address >> block_bits) & (1 << set_bits);
		target = memory_address >> (set_bits + block_bits);
		// cache match
		if(exist_v) {
			VCacheMatch(cache, set_address, target);
		}
		else {
			CacheMatch(cache, set_address, target);
		}	
		printf("the memory address is %x\n", memory_address);
		printf("the op is %s\n", op);
	}

    printSummary(hits, misses, evictions);
    return 0;
}
