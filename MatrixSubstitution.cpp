/*These formulas and ideas were found on:
1. Khan academy ray-tracing part 2 video segment [https://www.khanacademy.org/partner-content/pixar/rendering/rendering-2/v/rendering-10]
2. Linear Algebra class offered by the University of Texas at Austin through edX open courseware [https://www.edx.org/course/linear-algebra-foundations-to-frontiers-2].
One function a ray tracer performs is to determine if a ray intersects with a triangle in order to give the triangle correct lighting.
 Matrix substitution can be used to find out if a ray intersects with a triangle.	

	      A			      A \    
	    / \	\		    /	\    \   	
	   /   \b\		   /   c \   /	*
	  / c /	* \ 		  /	 /\  b  /
	 /  /	 \ \		 /    /	   \   /
	/ /	a  \\		/ /	a   \ /
	B-----------C		B------------C


The triangle on the left shows the intersection point inside of the triangle and how the weights of the points A, B, and C are positive. 
The triangle on the right shows that point B has a negative weight resulting in the intersection point being outside of the triangle.
From the depiction above, it can be seen that the weights of the points A, B, and C form the intersection point. 
Using the equation below, the weights of each of the points can be found. 
If any of them are negative, the conclusion is that the intersection point is outside of the triangle.
I = A + B + C [ I is the intersection point, A, B and C are points of the triangle.]
The original equation can be split up into three different equations to solve for the x, y, and z or the weights of the intersection point.
	Ix = Ax + Bx + Cx
	Iy = Ay + By + Cy
	Iz = Az + Bz + Cz

From there, the system of equations can be made into a matrix in order to solve for x, y, and z. 
Once x, y, and z are found their values will be checked to see if any of them are negative. 
If all the values are positive, the function can return true because the ray did intersect with the triangle.
*/	
	double Ix = triPoint.p[0]; double Iy = triPoint.p[1]; double Iz = triPoint.p[2];
	double Ax = t.top.p[0]; double Ay = t.top.p[1]; double Az = t.top.p[2];
	double Bx = t.right.p[0]; double By = t.right.p[1]; double Bz = t.right.p[2];
	double Cx = t.left.p[0]; double Cy = t.left.p[1]; double Cz = t.left.p[2];

	double Matrix[12] = { Ax, Bx, Cx,
			  	 Ay, By, Cy,
				 Az, Bz, Cz,
				 Ix, Iy, Iz };

/*Since every matrix in the function will always have 12 variables, the LU_factorization was written without matrix specific functions. 
A more elegant way to write the LU_factorization function would be to write matrix functions (i.e. matrix-matrix multiplication, addition, matrix-vector multiplication, addition functions, etc.), or to install a MatLab plugin to perform the matrix functions.
 
The matrix is passed into the LU_factorization function and will return an array with the weights of the points A, B, and C. 
The function then checks to see if any of the weights are negative and returns true or false depending on the if statements.
*/
	double *LU = LU_factorization(Matrix);

	if (LU[0] <= 0) {
		return false;
	}
	if (LU[1] <= 0) {
		return false;
	}
	if (LU[2] <= 0) {
		return false;
	}
	return true;
}
/*The LU_factorization function solves the system of equations 3 steps. 
The 3 steps are derived from the equation Ax = b. 
In the equation Ax = b, A is an nxn(square) matrix, while x is an unknown nx1 vector, and b is the resulting nx1 vector. 
A and b are known variables, for example if the triangle had the points (1, 2, -2), (-1, 0, 0), and (2, 2, 4) with an intersection point of (7, 4, -10) then the equations would be filled out as follows:
	1x + -1x + 2x = 7x     [Ax + Bx + Cx = Ix] 
	2y + 0y + 2y = 4y      [Ay + By + Cy = Iy]
 	-2x + 0y +4z  = -10z [ Az + Bz + Cz = Iz]

	[ 1   -1    2  
	  2    0     2 
 	 -2    0    4 ]         This is the nxn matrix A



	[ 7
  	  4
	 -10 ]	This is the resultant vector b.

	[ x
	  y
	  z ] This is the unknown vector x

When the [ x, y, z] vector is multiplied by the matrix A, the resultant vector will be the vector b. 
To find out what the vector x is, the equation Ax = b will be manipulated because the equation can be solved with a few tricks from special matrix types. 
The first thing to do is to split matrix A into two matrices, a lower unit triangular matrix and an upper triangular matrix. 
A unit lower triangular matrix can be solved by operating on the matrix until there are 2’s on the diagonal. 
So that is why step #1 is to split A into L and U (to save on memory, L and U will be stored in the same matrix because a lower unit triangular matrix and an upper triangular matrix do not overlap).
	#1. A = LU
Substituting LU in for A in the original formula:
	(LU)x = b
The parenthesis can be rearranged to show:
L(Ux) = b
Using substitution (substituting y for Ux) the equation can then be written:
	#2. Ly = b
That will be step #2, solving for y because the lower unit triangular system and b vector are known. 
Once that is solved, y will be left with the result that once the upper triangular system is solved, x will be known:
	#3. Ux = y

The first step in the algorithm is to find A = LU, a lower unit triangle matrix that can be multiplied by an upper triangle matrix to equal the matrix A. 
The way to find L is to manipulate the matrix so that it has 1’s on the diagonal. Once L is found, U will be everything above the diagonal of L.
 
The algorithm goes as follows: start at the first diagonal item in the matrix, (if it is 0, switch this row with the row below it because there needs to be a number here that can be made into a 1), divide the vector below the first item by the first item, multiply everything to the right of the first item by the divided vector and minus that answer from the rest of the matrix. 
An example will be shown to clear up the confusion. 
The following variable names to represent the matrix A are arbitrary, but are from a specific template used by the Linear Algebra course mentioned above.

	[ A00,  a01,     A02,
       	  a10t, alpha11, a12t,
          A20,  a21,     A22 ]

These are the two lines of code that performs the algorithm operations.
a21 = a21 / alpha11;
	
	A22 = A22 - a21 * a12t

The matrix will be worked on starting from top-left to bottom-right and each loop will execute the two lines of code from above. Using the points mentioned earlier:
 [ 1	-1	2  	 A00 = []  a01 = NaN     A02=[]
    2	 0 	2 	 a10t = NaN alpha11= 1 a12t = [ -1 2 ]
   -2	 0 	4 ] 	 A20 = []  a21 = [ 2; -2]   A22 = [ 0 2; 0 4 ]

The first check is made to see if alpha11 is 0. If it is 0, then it needs to be switched it with a row that doesn’t have a zero as its first number. 
(This could degrade if all rows in the matrix started out with zero, but weeding out bad data should prevent that).  
Since it’s not zero in this instance, a21 is set equal to a21/ alpha11 and A22 is set equal to A22 – a21 * a12t;

            [ 1       -1        2  	  a21 = [2 ; -2] / 1	
              2        0         2 	
             -2       0         4 ]	   A22 = [0 2; 0 4 ] -	([ 2 ; -2] * [-1 ;2 ]  )      
 				

	[1	-1	2	A00 = 1  a01 = -1     A02 = 2
	 2	-6	-4	a10t = 2 alpha11= -6 a12t = -4
	-2	-6	-2]	A20 = -2  a21 =  -6   A22 = -2
	
	[1	-1	2	a21 = -6/-6
	 2	-6	-4	A22 = -2 –(1 * -4)
	-2	-6	-2]	


	[1	-1	2	A00 = [1 -1; 2 6]  a01 = [2; -4] A02 = []
	 2	-6	-4	a10t = [-2 1] alpha11= 2 a12t = NaN
	-2	1	2]	A20 = []  a21 =  Nan   A22 = []

The first two iterations of the loop in code are outlined below.
*/
double * LU_factorization(double *Matrix) {
	//---------------Finding A = LU----------------------
	//---------------First iteration of loop-------------
	//if alpha11 is 0
	if (Matrix[0] == 0) {
double temp0 = Matrix[0]; double temp1 = Matrix[1]; double temp2 = Matrix[2];
		//switch the current row with the next row
		Matrix[0] = Matrix[3]; Matrix[1] = Matrix[4]; Matrix[2] = Matrix[5];
		Matrix[3] = temp0; Matrix[4] = temp1; Matrix[5] = temp2;
		//also switch b vector rows to match
		double temp3 = Matrix[9]; Matrix[9] = Matrix[10]; Matrix[10] = temp3;
	}
	//a21 = a21 / alpha11;
	Matrix[3] /= Matrix[0];
	Matrix[6] /= Matrix[0];
	//A22 = A22 - a21 * a12t
	double a21Xa12t[4] = { Matrix[1] * Matrix[3],
		               Matrix[2] * Matrix[3],
			       Matrix[1] * Matrix[6],
			       Matrix[2] * Matrix[6] };
	Matrix[4] -= a21Xa12t[0];
	Matrix[5] -= a21Xa12t[1];
	Matrix[7] -= a21Xa12t[2];
	Matrix[8] -= a21Xa12t[3];

	//--------------Second iteration of loop---------
	//if alpha11 is 0
	if (Matrix[4] == 0) {
		double temp0 = Matrix[3]; double temp1 = Matrix[4]; double temp2 = Matrix[5];
		//switch current row with row below
		Matrix[3] = Matrix[6]; Matrix[4] = Matrix[7]; Matrix[5] = Matrix[8];
		Matrix[6] = temp0; Matrix[7] = temp1; Matrix[8] = temp2;
		//switch b vector row too
		double temp3 = Matrix[10]; Matrix[10] = Matrix[11]; Matrix[11] = temp3;
	}
	//a21 = a21 / alpha11
	Matrix[7] /= Matrix[4];
	//A22 = A22 - a21 * a12t
	Matrix[8] -= Matrix[7] * Matrix[5];


/*After the two iterations of the loop, the ending matrix represents the LU matrix. The next step in the algorithm is to solve L(y) = b. 
This also has two loop iterations, but it involves less matrix functions. 
The L part of the matrix will be worked with and specifically the l21 variable. 
The l21 variable stores the number multiplied by that row to allow the subtraction to take place. 
Working from the top of vector b to the bottom, the changes made to the L matrix will be made to the b vector and will result in the y vector.

	 [ B0	
  	   B1	
	   B2  ]	b vector	 

	B2 = B2 – B1 * l21 is the equation used to apply the changes to the b vector. 
The matrix again will be worked on from top-left to bottom-right and the b vector will have three variables and will start at the top and work to the bottom.

[1	-1	2	  L00 = []  l01 = NaN     L02 = []
2	-6	-4	  l10t = NaN lamda11 = 1 l12 = [ -1 2 ]
-2	1	2]	  L20 = []  l21 = [ 2 ; -2]     L22 = [ -6 -4; 1 2 ]

	[ 7		  B0 = NaN	
  	  4		  B1 = 7
	 -10 ]		  B2 = [ 4; -10 ]

	[ 7                 B2 = [ 4; -10 ] – ( 7 * [ 2 ; -2 ] )
	  4
   	-10 ]


[1	-1	2	  L00 = 1  l01 = -1     L02 = 2
2	-6	-4	  l10t = 2 lamda11 = -6 l12 = -4
-2	1	2]	  L20 = -2  l21 = 1     L22 = 2

	[ 7		  B0 = 7	
  	  4		  B1 = 4
	 -10 ]		  B2 = -10

	[ 7                 B2 =  -10 – ( 4 * 1)
	  4
   	-10 ]

	[ 7                 B2 =  -10 – ( 4 * 1)
	  4
   	-14 ]


The code below outlines what was just shown.
*/
	//------------Performing L(y) = b-------------
	//B2 = B2 - B1 * l21
	Matrix[10] -= Matrix[9] * Matrix[3];
	Matrix[11] -= Matrix[9] * Matrix[6];
	//B2 = B2 - B1 * l21
	Matrix[11] -= Matrix[10] * Matrix[7];

/*There is only one more step to complete before x, y, and z are found (remember, that’s the weights of each point). 
The last step is to find the vector x by solving U(x) = b. 
What was previously found was y. 
The y vector represents U multiplied by x, so solving the upper triangular system U, will leave the vector x. 
In this instance the U matrix will be worked on from bottom-right to top-left and the vector b will be worked on from bottom to top. 
The two algorithms that will be used for this function are B1 = B1 – u12t * B2 and B1 = B1 / gamma11.

[1	-1	2	  U00 = [ 1 -1; 2 -6 ]  u01 = [ 2; -4 ]   U02 = []
2	-6	-4	  u10t = [ -2 1 ] gamma11 = 2 u12t = NaN
-2	1	2]	  U20 = []  u21 = NaN     U22 = []

	[ 7		  B0 =	[ 7; 4 ]
  	  4		  B1 = -14
	 -14 ]		  B2 = NaN

	[ 7                 u12t is not filled on the first iteration
	  4	         B1 = -14 / 2
   	-14 ]



[1	-1	2	  U00 = 1  u01 = -1   U02 = 2
2	-6	-4	  u10t = 2 gamma11 = -6 u12t = -4
-2	1	2]	  U20 = -2  u21 = 1     U22 = 2

	[ 7		  B0 = 7	
  	  4		  B1 = 4
	 -7]		  B2 = -7

	[ 7               B1 = 4 – (-4 * -7)
	  4	          B1 = -24 / -6
   	-7 ]


[1	-1	2	  U00 = []  u01 = NaN   U02 = []
2	-6	-4	  u10t = NaN gamma11 = 1 u12t = [ -1 2 ]
-2	1	2]	  U20 = []  u21 = [ 2 ; -2 ]    U22 = [ -6 -4; 1 2 ]

	[ 7		  B0 = NaN	
  	  -24/6		  B1 = 7
	 -7]		  B2 = [ -24/6 ; -7 ]

	[ 7               B1 = 7 – ([ -1 2 ] * [ -24 /6; 7]
	  -24 /6	  B1 = 18 / 1
   	-7 ]


	[ 7               = x
	  -24 /6	  = y
   	18 ]              = z

The code for completing U(x) = b is below and the vector x is stored in an array and returned to the main function.
*/
	//------------Performing U(x) = b--------------

	//B1 = B1 / gamma11;
	Matrix[11] /= Matrix[8];

	//B1 = B1 - u12t * B2;
	Matrix[10] -= Matrix[5] * Matrix[11];
	//B1 = B1 / gamma11;
	Matrix[10] /= Matrix[4];

	//B1 = B1 - u12t * B2;
	Matrix[9] -= (Matrix[1] * Matrix[10]) + (Matrix[2] * Matrix[11]);
	//B1 = B1 / gamma11;
	Matrix[9] /= Matrix[0];

	//return the correct factorization
	double Answer[3] = { Matrix[9], Matrix[10], Matrix[11] };
	return Answer;
}
/*
This was shown earlier but this is where the LU factorization function will return to. 
The last step is to check if x, y or z are less than or equal to zero and then the function can return true or false.*/
double *LU = LU_factorization(Matrix);
	if (LU[0] <= 0) {
		return false;
	}
	if (LU[1] <= 0) {
		return false;
	}
	if (LU[2] <= 0) {
		return false;
	}
	return true;
}
/*Three things to remember to do before the LU_factorization function is implemented. 
The first thing is to make sure that the data that is read in contains triangles.
 Data may contain lines or repeated points instead of triangles.
 
The first thing to check is if any of the triangles’ points are the same	
*///-----------Triangle Check-------------
	//this checks if any points are the same
	if (Ax == Bx && Ay == By && Az == Bz) {
		return false;
	}
	if (Ax == Cx && Ay == Cy && Az == Cz) {
		return false;
	}
	if (Bx == Cx && By == Cy && Bz == Cz) {
		return false;
	}
/*Then there is a simple check that can be done to see if the points form a line instead of a triangle.
 One way to do that is to get the length of each of the sides and make sure that the hypotenuse is not equal to the two sides added up. 
(Another way to do this that is different from the code below is just to find the hypotenuse and then do one check).
*/
	//this checks to see if points make a line instead of triangle
	double sideA = side2.Length();
	double sideB = side1.Length();
	double sideC = side3.Length();
	if (sideA + sideB == sideC) {
		return false;
	}
	if (sideB + sideC == sideA) {
		return false;
	}
	if (sideA + sideC == sideB) {
		return false;
	}
	//-----------------------------------

/*Another quick note is to make sure that the object read in is located in the upper right quadrant. 
Every point needs to start out positive in order to correctly determine the weights of the triangle. 
One way to do that is to find the lowest points in the data for the x, y, and z direction and then add the absolute value of those lowest points to every x, y, and z point as they are read in.*/
