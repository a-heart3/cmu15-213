/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
// switch table store in the memory, do not use this
void transpose_32x32_switch(int M, int N, int A[N][M], int B[M][N]) 
{
	int turn;
	int row_start;
	int column_start;
	int temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;
	// split matrix to 8x8 matrix
	for(turn=0; turn<16; turn++) {
		row_start = (turn / 4) * 8;
		column_start = (turn % 4) * 8;
		for(M=row_start; M<row_start+8; M++) {
			for(N=column_start; N<column_start+8; N++) {
				switch(N % 8) {
					case 0: temp0 = A[M][N]; break;
					case 1: temp1 = A[M][N]; break;
					case 2: temp2 = A[M][N]; break;
					case 3: temp3 = A[M][N]; break;
					case 4: temp4 = A[M][N]; break;
					case 5: temp5 = A[M][N]; break;
					case 6: temp6 = A[M][N]; break;
					case 7: temp7 = A[M][N]; break;
				}
			}
			for(N=column_start; N<column_start+8; N++) {
				switch(N % 8) {
					case 0: B[N][M] = temp0; break;
					case 1: B[N][M] = temp1; break;
					case 2: B[N][M] = temp2; break;
					case 3: B[N][M] = temp3; break;
					case 4: B[N][M] = temp4; break;
					case 5: B[N][M] = temp5; break;
					case 6: B[N][M] = temp6; break;
					case 7: B[N][M] = temp7; break;
				}
			}
		}
	}
}

void transpose_32x32(int M, int N, int A[N][M], int B[M][N]) 
{
	int turn;
	int row_start;
	int column_start;
	int temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;
	// split matrix to 8x8 matrix
	for(turn=0; turn<16; turn++) {
		row_start = (turn / 4) * 8;
		column_start = (turn % 4) * 8;
		for(M=row_start; M<row_start+8; M++) {
			N = column_start;
			temp0 = A[M][N]; N = N + 1;
			temp1 = A[M][N]; N = N + 1;
			temp2 = A[M][N]; N = N + 1;
			temp3 = A[M][N]; N = N + 1;
			temp4 = A[M][N]; N = N + 1;
			temp5 = A[M][N]; N = N + 1;
			temp6 = A[M][N]; N = N + 1;
			temp7 = A[M][N]; N = N + 1;

			N = column_start;
			B[N][M] = temp0; N = N + 1;
			B[N][M] = temp1; N = N + 1;
			B[N][M] = temp2; N = N + 1;
			B[N][M] = temp3; N = N + 1;
			B[N][M] = temp4; N = N + 1;
			B[N][M] = temp5; N = N + 1;
			B[N][M] = temp6; N = N + 1;
			B[N][M] = temp7; N = N + 1;
		}
	}
}

void transpose_64x64(int M, int N, int A[N][M], int B[M][N]) 
{
	int turn;
	int row_start;
	int column_start;
	int temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;
	// split matrix to 8x8 matrix
	for(turn=0; turn<128; turn++) {
		row_start = (turn / 16) * 8;
		column_start = (turn % 16) * 4;
		for(M=row_start; M<row_start+4; M++) {
			N = column_start;
			temp0 = A[M][N]; N = N + 1;
			temp1 = A[M][N]; N = N + 1;
			temp2 = A[M][N]; N = N + 1;
			temp3 = A[M][N]; N = N + 1;

			M += 4;
			N = column_start;
			temp4 = A[M][N]; N = N + 1;
			temp5 = A[M][N]; N = N + 1;
			temp6 = A[M][N]; N = N + 1;
			temp7 = A[M][N]; N = N + 1;

			M -= 4;
			N = column_start;
			B[N][M] = temp0; N = N + 1;
			B[N][M] = temp1; N = N + 1;
			B[N][M] = temp2; N = N + 1;
			B[N][M] = temp3; N = N + 1;

			M += 4;
			N = column_start;
			B[N][M] = temp4; N = N + 1;
			B[N][M] = temp5; N = N + 1;
			B[N][M] = temp6; N = N + 1;
			B[N][M] = temp7; N = N + 1;
			
			M -= 4;
		}
	}
}

void transpose_64x64_v2(int M, int N, int A[N][M], int B[M][N])
{
	int turn;
	int row_start;
	int column_start;
	int temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;
	for(turn = 0; turn < 64; turn++) {
		row_start = (turn / 8) * 8;
		column_start = (turn % 8) * 8;
		for(M=row_start; M<row_start+8; M++) {
			N = column_start;
			// get one block A
			temp0 = A[M][N]; N++;
			temp1 = A[M][N]; N++;
			temp2 = A[M][N]; N++;
			temp3 = A[M][N]; N++;
			temp4 = A[M][N]; N++;
			temp5 = A[M][N]; N++;
			temp6 = A[M][N]; N++;
			temp7 = A[M][N]; 
			// give the value to B, split two cases, one is row 0-3; another is row 4-7
			if(M < row_start+4) {
				N = column_start;
				B[N][M] = temp0; N++;
				B[N][M] = temp1; N++;
				B[N][M] = temp2; N++;
				B[N][M] = temp3; 

				M = M + 4;
				B[N][M] = temp7; N--;
				B[N][M] = temp6; N--;
				B[N][M] = temp5; N--;
				B[N][M] = temp4;

				M = M - 4;
			}
			else {
				M = M - 4;
				N = column_start + 4;
				B[N][M] = temp0; N++;
				B[N][M] = temp1; N++;
				B[N][M] = temp2; N++;
				B[N][M] = temp3;
				
				M = M + 4;
				B[N][M] = temp7; N--;
				B[N][M] = temp6; N--;
				B[N][M] = temp5; N--;
				B[N][M] = temp4;
			}
		}
		
		for(N=column_start; N < column_start+4; N++) {
			M = row_start;
			temp4 = B[N+4][M]; M = M + 1;
			temp5 = B[N+4][M]; M = M + 1;
			temp6 = B[N+4][M]; M = M + 1;
			temp7 = B[N+4][M]; M = M + 1; 

			temp0 = B[N][M]; M = M + 1;
			temp1 = B[N][M]; M = M + 1;
			temp2 = B[N][M]; M = M + 1;
			temp3 = B[N][M]; 
			
			M = M - 3;
			B[N][M] = temp4; M = M + 1;
			B[N][M] = temp5; M = M + 1;
			B[N][M] = temp6; M = M + 1;
			B[N][M] = temp7; 
			
			M = M - 7;
			B[N+4][M] = temp0; M = M + 1;
			B[N+4][M] = temp1; M = M + 1;
			B[N+4][M] = temp2; M = M + 1;
			B[N+4][M] = temp3; M = M + 1;
		}
	}
}

void transpose_64x64_v4(int M, int N, int A[N][M], int B[M][N])
{
	int turn;
	int row_start;
	int column_start;
	int temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;
	for(turn = 0; turn < 64; turn++) {
		row_start = (turn / 8) * 8;
		column_start = (turn % 8) * 8;
		for(M=row_start; M<row_start+4; M++) {
			N = column_start;
			// get one block A
			temp0 = A[M][N]; N++;
			temp1 = A[M][N]; N++;
			temp2 = A[M][N]; N++;
			temp3 = A[M][N]; N++;
			temp4 = A[M][N]; N++;
			temp5 = A[M][N]; N++;
			temp6 = A[M][N]; N++;
			temp7 = A[M][N]; 
			// give the value to B, split two cases, one is row 0-3; another is row 4-7
			N = column_start;
			B[N][M] = temp0; N++;
			B[N][M] = temp1; N++;
			B[N][M] = temp2; N++;
			B[N][M] = temp3; 

			M = M + 4;
			B[N][M] = temp7; N--;
			B[N][M] = temp6; N--;
			B[N][M] = temp5; N--;
			B[N][M] = temp4;

			M = M - 4;
		}
		for(N=column_start; N<column_start+4; N++) {
			M = row_start + 4;
			temp0 = A[M][N]; M++;
			temp1 = A[M][N]; M++;
			temp2 = A[M][N]; M++;
			temp3 = A[M][N];

			temp7 = B[N][M]; M--;
			temp6 = B[N][M]; M--;
			temp5 = B[N][M]; M--;
			temp4 = B[N][M];

			B[N][M] = temp0; M++;
			B[N][M] = temp1; M++;
			B[N][M] = temp2; M++;
			B[N][M] = temp3;

			M = M - 7;
			B[N+4][M++] = temp4;
			B[N+4][M++] = temp5;
			B[N+4][M++] = temp6;
			B[N+4][M] = temp7;
		}
		for(M=row_start+4; M<row_start+8; M++) {
			N = column_start+4;
			temp0 = A[M][N++];
			temp1 = A[M][N++];
			temp2 = A[M][N++];
			temp3 = A[M][N];

			B[N][M] = temp3; N--;
			B[N][M] = temp2; N--;
			B[N][M] = temp1; N--;
			B[N][M] = temp0;
		}

	}
}


void transpose_64x64_v3(int M, int N, int A[N][M], int B[M][N])
{
	int turn;
	int row_start;
	int column_start;
	int temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;
	for(turn = 0; turn < 64; turn++) {
		row_start = (turn / 8) * 8;
		column_start = (turn % 8) * 8;
		for(M=row_start; M<row_start+8; M++) {
			N = column_start;
			// get one block A
			temp0 = A[M][N]; N++;
			temp1 = A[M][N]; N++;
			temp2 = A[M][N]; N++;
			temp3 = A[M][N]; N++;
			temp4 = A[M][N]; N++;
			temp5 = A[M][N]; N++;
			temp6 = A[M][N]; N++;
			temp7 = A[M][N]; 
			// give the value to B, split two cases, one is row 0-3; another is row 4-7
			if(M < row_start+4) {
				N = column_start;
				B[N][M] = temp0; N++;
				B[N][M] = temp1; N++;
				B[N][M] = temp2; N++;
				B[N][M] = temp3; 

				M = M + 4;
				B[N][M] = temp7; N--;
				B[N][M] = temp6; N--;
				B[N][M] = temp5; N--;
				B[N][M] = temp4;

				M = M - 4;
			}
			else {
				M = M - 4;
				N = column_start + 4;
				B[N][M] = temp0; N++;
				B[N][M] = temp1; N++;
				B[N][M] = temp2; N++;
				B[N][M] = temp3;
				
				M = M + 4;
				B[N][M] = temp7; N--;
				B[N][M] = temp6; N--;
				B[N][M] = temp5; N--;
				B[N][M] = temp4;
			}
		}
		
		for(N=column_start; N < column_start+4; N++) {
			M = row_start;
			temp0 = B[N+4][M]; M = M + 1;
			temp1 = B[N+4][M]; M = M + 1;
			temp2 = B[N+4][M]; M = M + 1;
			temp3 = B[N+4][M]; M = M + 1;

			temp4 = B[N][M]; B[N][M] = temp0; M = M + 1;
			temp5 = B[N][M]; B[N][M] = temp1; M = M + 1;
			temp6 = B[N][M]; B[N][M] = temp2; M = M + 1;
			temp7 = B[N][M]; B[N][M] = temp3; 

			M = row_start;
			B[N+4][M] = temp4; M = M + 1;	
			B[N+4][M] = temp5; M = M + 1;	
			B[N+4][M] = temp6; M = M + 1;	
			B[N+4][M] = temp7; 
		}
	}
}


void transpose_61x67(int M, int N, int A[N][M], int B[M][N])
{
	int turn;
	int row_start, column_start;
	int temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;
	
	for(turn = 0; turn < 56; turn++) {
		row_start = (turn / 7) * 8;
		column_start = (turn % 7) * 8;
		for(M=row_start; M<row_start+8; M++) {
			N = column_start;
			temp0 = A[M][N]; N++;
			temp1 = A[M][N]; N++;
			temp2 = A[M][N]; N++;
			temp3 = A[M][N]; N++;
			temp4 = A[M][N]; N++;
			temp5 = A[M][N]; N++;
			temp6 = A[M][N]; N++;
			temp7 = A[M][N]; 
			
			B[N][M] = temp7; N--;
			B[N][M] = temp6; N--;
			B[N][M] = temp5; N--;
			B[N][M] = temp4; N--;
			B[N][M] = temp3; N--;
			B[N][M] = temp2; N--;
			B[N][M] = temp1; N--;
			B[N][M] = temp0; 
		}
	}
	for(turn = 0; turn < 8; turn++) {
		row_start = (turn % 8) * 8;
		column_start = 56;
		for(M = row_start; M<row_start+8; M++) {
			N = column_start;
			temp0 = A[M][N]; N++;
			temp1 = A[M][N]; N++;
			temp2 = A[M][N]; N++;
			temp3 = A[M][N]; N++;
			temp4 = A[M][N]; 
			
			B[N][M] = temp4; N--;
			B[N][M] = temp3; N--;
			B[N][M] = temp2; N--;
			B[N][M] = temp1; N--;
			B[N][M] = temp0; 
		}
	}
	for(turn = 0; turn < 7; turn++) {
		row_start = 64;
		column_start = (turn % 7) * 8;
		for(M = row_start; M < row_start+3; M++) {
			N = column_start;
			temp0 = A[M][N]; N++;
			temp1 = A[M][N]; N++;
			temp2 = A[M][N]; N++;
 			temp3 = A[M][N]; N++;
			temp4 = A[M][N]; N++;
			temp5 = A[M][N]; N++;
			temp6 = A[M][N]; N++;
			temp7 = A[M][N]; 

			
			B[N][M] = temp7; N--;
			B[N][M] = temp6; N--;
			B[N][M] = temp5; N--;
			B[N][M] = temp4; N--;
			B[N][M] = temp3; N--;
			B[N][M] = temp2; N--;
			B[N][M] = temp1; N--;
			B[N][M] = temp0; 

		}
	}
	for(M=64; M < 67; M++) {
		N = 56;
		temp0 = A[M][N]; N++;
		temp1 = A[M][N]; N++;
		temp2 = A[M][N]; N++;
		temp3 = A[M][N]; N++;
		temp4 = A[M][N]; 
			
		B[N][M] = temp4; N--;
		B[N][M] = temp3; N--;
		B[N][M] = temp2; N--;
		B[N][M] = temp1; N--;
		B[N][M] = temp0; N--;
	}	
}	


char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	if((M==32) && (N==32)) {
		transpose_32x32(M, N, A, B);
	}
	else if((M==64) && (N==64)) {
		transpose_64x64_v4(M, N, A, B);
	}
	else {
		transpose_61x67(M, N, A, B);
	}
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

