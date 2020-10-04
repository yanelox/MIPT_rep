#include <stdio.h>
#include <math.h>
#include <assert.h>

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#define and &&
#define or ||

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/*!
	@mainpage
	
	@author jkoer

	@brief This programm solves the square equation
	
	@version 0.0	

	@param [in] mode Selected work mode

	@param [in] a a - coefficient

	@param [in] b b - coefficient
	
	@param [in] c c - coefficient

	@param [out] x1 First root of equation
	
	@param [out] x2 Second root of equation 
*/

/*!
	Special value which will be returned by EquationSolver function if equation has infinity count of roots
*/

#define INFINITY_ROOTS_VALUE 42

/*!
	Accuracy of comparing values with zero. It is used in IsZero() function
*/

const double ACCURACY = 1e-6;

/*!
	Solves a square equation like ax^2 + bx + c = 0
	
	@param [in] a a - coefficient
	@param [in] b b - coeffecient
	@param [in] c c - coefficient

	@param [in] x1 Pointer to variable of first root of equation
	@param [in] x2 Pointer to variable of second root of equation 

	@return Count of solutions
    	
	@note
	- Returns NUMBER_INF_COUNT in case
        of infinity count of roots
 	- If equation have only one
        solution, it will be saved in x1 and
        x2
*/

int EquationSolver (double   a, double   b, double c,
                    double* x1, double* x2);

/*!
	Test an "EquationSolver" function
	
	@param [in] fp Variable with type FILE* which contains information about file-name with test values
	
	@note Print "OK" if test complete succesfully and "FALSE" and information about error in case of some errors
*/

void EquationSolver_test (FILE* fp);

/*!
	Compares values with zero with some accuracy

	@param [in] a Value which we want to compare

	@return 1 if number is comparable with zero with accuracy and 0 if it is not 
*/

int IsZero (double a);

/*!
	Input 1 double value
	
	@param [in] n Pointer to value which you want to input
	@param [in] a String which will be printed before input in format: "Please, input (a):"

	@note There is checking for type of input value. If you input incorrect you will be asked to input value again 
*/

void Input_Coef (char a[], double* n);

/*!
	Choosing of working mode

	@note Asked for int value, 0 if you want to work in test mode or 1 if you wat to work in solver mode
*/

int ChooseMode (void);

/*!
	Checking definition to true/false and print line_number in "false" case

	@param [in] definition Integet varuable 1 or 0 (use ternary operators to create a definition)
	@param [in] line_number You should put __LINE__ in any time here
!*/

void ErrorHandler (int definition, int line_number);

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

int main ()
{
	FILE* fp = fopen("test.txt", "r");  // name of test-values file

	double a = 0;
	double b = 0;
	double c = 0;

	double x1 = 0;
	double x2 = 0;

	int countSol = -1;

	int mode = ChooseMode ();	

	if (mode == 0)
		EquationSolver_test (fp);

	else if (mode == 1)
	{
		Input_Coef ("a", &a);
	
		Input_Coef ("b", &b);

		Input_Coef ("c", &c);

		countSol = EquationSolver ( a, b, c, &x1, &x2);


		switch (countSol)
		{
			case 2:  
				printf("x1 = %lg\n" "x2 = %lg\n", x1, x2);
				break;

			case 1: 
				printf("x = %lg", x1);
				break;

			case 0:  
				printf("No real roots");
				break;

			case INFINITY_ROOTS_VALUE:  
				printf("Any number");
				break;

			default: 
				printf("Error: countSol = %d\n", countSol);
				return 1;
		}

	}

	else
		printf ("ERROR: Unknown mode, you have to input 0 or 1. Your input: %d", mode);

}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

int EquationSolver (double   a, double   b, double c,
                    double* x1, double* x2)
{
	assert (isfinite (a));
	assert (isfinite (b));
	assert (isfinite (c));

	assert (x1 != NULL);
	assert (x2 != NULL);
	assert (x1 != x2);

	if (!IsZero(a))
	{
		b /= a;
		c /= a;
		a = 1;
	}
	
	double disc = b * b - 4 * a * c;

	ErrorHandler (isfinite(disc) ? 1 : 0, __LINE__);

	if ((IsZero(a) and IsZero(b) and !IsZero(c)) or (!IsZero(a) and disc < 0))
		return 0;

	else if ((IsZero(a) and !IsZero(b)))
	{
		*x1 = -c / b;
		*x2 = -c / b;
		
		return 1;
	}

	else if (!IsZero(a) and IsZero(disc))
	{
		*x1 = -b / (2 * a);
		*x2 = -b / (2 * a);

		return 1;
	}
	
	else if (!IsZero(a) and disc > 0)
	{
		*x1 = (-b + sqrt(disc)) / (2 * a);
		*x2 = (-b - sqrt(disc)) / (2 * a);
		
		return 2;
	}
	
	else if (IsZero(a) and IsZero(b) and IsZero(c))
	{
		return INFINITY_ROOTS_VALUE;
	}
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void EquationSolver_test (FILE* fp)
{
   	double a = 0;                               //a, b, c coefficients
	double b = 0;
	double c = 0;

	double true_x1 = 0;                    	    // true_x1, true_x2 - numbers from
	double true_x2 = 0;    			    // file, true roots for current test
										// equation
	double x1 = 0;                              // x1, x2 - calculated roots
	double x2 = 0;

	int count = 0;                              // count - count of calculated roots

	int n = 0, k = 0, i = 0;                    // n - first number in file, count of tests

	fscanf (fp, "%d", &n);

	for (i = 0; i < n; i++)
	{
		k = fscanf (fp, "%lg %lg %lg %lg %lg", &a, &b, &c, &true_x1, &true_x2);

		if (k != 5)                             // if all input values correct fscanf returns 5
		{
			printf ("Test %d: Incorrect input\n", i + 1);
		}	

		else
		{
			count = EquationSolver (a, b, c, &x1, &x2); // calculating roots

			if ((x1 == true_x1 and x2 == true_x2) or (x1 == true_x2 and x2 == true_x1))
					printf ("Test %d: OK\n", i + 1);

			else
			{
				printf ("Test %d: FALSE\n{\n", i + 1);

				printf ("True values: %lg, %lg\n", true_x1, true_x2);

				printf ("Count of calculated values: %d\n", count);

				printf ("Calculated values: %lg, %lg\n}\n", x1, x2);
			}
		}
	}
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

int IsZero (double a)
{
	return fabs (a) < ACCURACY;
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void Input_Coef (char a[], double* n)
{
	int k = 0;

	printf("Please, input %s (double vlaue):\n", a);

	while (k != 1)
	{
		k = scanf ("%lg", n);

		if (k != 1)
		{
			printf ("Incorrect input, please, try again\n");

			fflush (stdin);
		}
	}
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

int ChooseMode (void)
{
   	int mode = -1;
    int n = 0;


    printf ("Choose mode: write 0 to test mode or 1 for solver mode:\n");

    while (mode != 0 and mode != 1)
	{
		n = scanf ("%d", &mode);

		if ((mode != 0 and mode != 1) or n != 1)
		{
				printf("Incorrect mode, please, try again\n");
		}

		fflush (stdin);
	}

	return mode;        // mode == 0 => test-mode, mode == 1 => solver-mode
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void ErrorHandler (int definition, int line_number)
{
	if (line_number > 0)
	{
		if (!definition)
			printf ("Error in line: %d. Pls, call developers!!\n", line_number);
	}

	else
		printf ("Incorrect line number in func ErrorHandler: line_number = %d\n", line_number);
}