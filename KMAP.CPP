/*
		  Program to find the Minterms using a Karnaugh Map


   - According to Wikipedia a K Map is a method to simplify Boolean algebra expresion. It reduces the need for extensive calculation by taking advantage of "humans' pattern recognition capability".


   This program is an effort to convert this "pattern recognising capability" of humans into a computer code and hence printing the appropriate output.

 * Disclaimer: Before going into the code, I would like to specify that this code works only for cases with 4 variables and gives output in forms of Minterms.Also the map-rolling functions are still under progress as they work for basic cases but give redundant results in advanced answers.


- Parts of the code:

    The program below is divided into the following parts:

	 1) Taking the positions in the K Map which have high outputs from the user.

	 2) Plotting the K Map with the information provided by the user.

	 3) Appropriate re-arrangement of the K Map to satisfy anomalies.

	 4) To search for each pattern(ie Octates,Quads ) in the K Map.

	 5) To remove overlapping patterns or redundancies.

	 6) To perform Map Rolling wherever possible.

	 7) To Print the output in form of Minterms.



  A K Map has positions as shown below:


       0   I  II  III
    *  *   *   *   *          - Note the anomaly in the II Row and II Column.

0   *  0   1   3   2          - To account for this peculiar behavior, the II and III column are swaped to obtain a array as shown.(The exact array is saved in r[][] in this program)

I   *  4   5   7   6

II  *  12  13  15  14

III *  8   9  11  10


  Redundancy Removal: In the following exmaple,

  1  1  0  0           - A square Quad is formed in the positions, {0,1,4,5}
  1  1  0  0           - But this square quad consists of 2 horizontal and 2 verticle quads respectively.
  0  0  0  0
  0  0  0  0           - The Compiler will read this as:

				* Sqare Quad in {0,1,4,5}
				* Horizontal pairs {0,1} & {4,5}
				* Verticle pairs {0,4} & {1,5}

		       - The pairs are redundant and therefore must not be counted.
		       - A function check() is used which takes in two 1 D arrays and checks if one of the array is the subset of the other. If it is a subset then it is taken as redundant and not printed.

  - Map Rolling(Explained further within the code):

      - Maps can be rolled either vertically or horizontally.
      - Once a map is rolled, further patterns are obtained.
      - After rotation, Octates, Quads and pairs are obtained.



  */
#include<iostream.h>
#include<conio.h>
#include<stdio.h>


int roll;
int vo[8]={50,50,50,50,50,50,50,50};
int ho[8]={50,50,50,50,50,50,50,50};
int sqquadpos[12]={50,50,50,50,50,50,50,50,50,50,50,50};
int hlpos[8]={50,50,50,50,50,50,50,50 };
int vlpos[8]={50,50,50,50,50,50,50,50 } ;
int vl[8]={50,50,50,50,50,50,50,50};
int hl[8]={50,50,50,50,50,50,50,50};

// Functions used:				     // What they do(brief)

int binary(int);                                     // Converts a Decimal into a Binary
int lsearch(int, int[],int);                         //(linear Search) Searches a number in a array of given size
int xcomp(int);                                      //(X- Component) Finds the X Component of a given binary and stores it in the (n+1)th Row.
int ycomp(int);                                      //(Y- Component) Finds the Y Component of a given binary and stores it in the (n+1)th Column.
void voct(int,int[10][10],int[10][10]);             //(Verticle Octate) A function to check for  Veticle Octate.
void hoct(int,int[10][10],int[10][10]);              //(Horizontal Octate) A function to check for Horizontal Octate.
void lvquad(int[10][10],int,int[10][10]);            //(Linear Verticle Quad) A function to check for a Linear Verticle Quad.
void lhquad(int[10][10],int,int[10][10]);            //Linear Horizontal Quad) A function to check for a Linear Horizontal Quad.
void quad(int[10][10],int,int[10][10]);              // A function to check for Square Quads
void vpair(int[10][10],int,int[10][10]);             // A function to check for Verticle pairs
void hpair(int[10][10],int,int[10][10]);             // A function to check for Horizontal pairs
int check(int[20],int[20],int,int);                  //(Redundancy removing function)
void print(int[10],int);                             // A function to print the output
int maproll(int[10][10],int,int[10][10]);            // A function to check for quads using map roling of the four extreme corners of the K-Map
void vmaproll(int[10][10],int,int[10][10]);          // (Verticle map rolling)
void hmaproll(int[10][10],int,int[10][10]) ;         // (Horizomtal map rolling)


void main()
{
     clrscr();

int i=-1,x[30],n,judge=0,temp2,j,k[10][10],temp,unified,btemp,r[10][10];

cout<<" \n\n Enter the POS form  and 100 to teminate input";

     do
     {
       i++;
      cin>>x[i];                          //Input from the User about the positions at which the output is high.

      }while(x[i]!=100);

      int size=i;

 cout<<"\n\n Enter the number of variables involved \n";
   cin>>n;                            // Information about the number of variables from the user(This program works only for cases with 4 variables)

 if(n==4)
 { 			      // In this double-for loop,three things are done:
					//  1:  The array- r is fed values that are used in the further stages of the program.
     for(i=0;i<n;i++)                   //  2:  The binary equivalent is found and assigned to Variable positions and are stored in the (n+1)th row and column of the K Map. Explained in detail in the start of the code.
     {                                  //  3:  The values 1 or 0 are stored into each postion of the K Map using the function- lsearch.
	for(j=0;j<n;j++)
	{
	       r[i][j]=judge;
	  unified=  binary(judge);
	 k[n+1][j]= xcomp(unified);
	 k[i][n+1]= ycomp(unified);

	 k[i][j]= lsearch(judge,x,size);
	     judge++;                     // Judge is just a variable that increments every time the j- loop iterates.
	 }
      }

		       // The postions in a K-Map are marked as shown in the following.

      for(i=0;i<n;i++)
      {					//       0  I  II III   Bin
       for(j=0;j<n;j++)                 //     *  *  *  *  *
       {                                // 0   *  0  1  3  2     00
	   if(j==n-2)                   // I   *  4  5  8  7     01
	 {                              // II  * 12 13 15 14     11
	    temp=k[i][j];               // III *  8  9 11 10     10
	    k[i][j]=k[i][j+1];
	    k[i][j+1]=temp;             //Bin:   00 01 11 10

					// Note the anomaly in the 2nd row and 2nd column(counting assumed to start from 0).
					// The positions are switched.
	   temp2=r[i][j];               //  The corresponding code adjusts the Map so that this anomaly is accounted for.
					// Also there is anomaly in Bin marked row and colunm.
	     r[i][j]=r[i][j+1];
	   r[i][j+1]=temp2;
	  }                         // Column beign swaped.
	}
       }



     for(i=0;i<n;i++)
      {
       for(j=0;j<n;j++)
       {
	  if(i==n-2)                             // 2nd Row being swaped
	  {
	    temp=k[i][j];
	    k[i][j]=k[i+1][j];
	    k[i+1][j]=temp;

	    temp2=r[i][j];
	    r[i][j]=r[i+1][j];
	    r[i+1][j]=temp2;
	   }
       }
     }

       btemp=k[2][n+1];
       k[2][n+1]=k[3][n+1];
       k[3][n+1]=btemp;
					    // Binary components being swaped
      btemp=k[n+1][2];
      k[n+1][2]=k[n+1][3];
      k[n+1][3]=btemp;

 cout<<"\n\n  The K - Map plotted using the information provided above is: \n\n";

for(i=0;i<n;i++)
 {
   for(j=0;j<n;j++)
     {                                        //  K-Map printing
      cout<<" "<<k[i][j];
     }
     cout<<"\n";
  }


      //  Everything has been swaped and in place for checking of patterns

  cout<<"\n\n  Represention using Minterms by Extrapolation of the above data using a process similar to a K - Map is: \n\n\n ";

   voct(n,k,r);                // Check for Verticle Octate.

   hoct(n,k,r);                // Check for Horizontal Octate.

   quad(k,n,r);                 // Check for Square Quad.

   lvquad(k,n,r);               // Check for Linear Verticle Quad.

   vpair(k,n,r);                // Check for Verticle Pair.

   lhquad(k,n,r);               // Check for Linear Horizontal Quad.

   hpair(k,n,r);                // Check for horizontal Pair.

//   roll= maproll(k,n,r);        // Map Rolling functions.

//   vmaproll(k,n,r);             // Map rolling fuction.    These map rolling functions are disabled as they are giving proper answers for basic cases, but for advanced cases, they are giving redundanct results.

//   hmaproll(k,n,r);             // Map rolling function.

  }

  else if(n==2)
  {

     for(i=0;i<n;i++)
     {
	for(j=0;j<n;j++)
	{
	 k[i][j]= lsearch(judge,x,size);
	     judge++;
	 }
      }
  }

 else
{
   cout<<" under contruction";
    }

  cout<<"\b ";

  getch();

  }




 int lsearch(int judge,int x[],int size) // This function acts as both as a
					 //search and a fuction to feed
					 //values to the K-Map.
 {
    int i,flag=0;

     for(i=0;i<size;i++)        //In the main(),postions with high outputs
				//were stored in the array x.
      {
	if(x[i]==judge)       // Here each element of the array x is searched
			      // with judge,which increments in every
	 {                    // iteration in the main().
	 flag=1;              // If the number is found 1 is returned
	   }                  // or else 0 is returned.
       }
       if(flag==1)
	{
	return 1;
	  }

      else
	{
       return 0;
	}
   }


int binary(int pos)
{                                 // A fuction that converts a decimal into
				   // binary. The number to be converted is
				   // represented as 'pos' in this function.
	 int bin=0;                // The binary of 'pos' is returned.
	  int multi=1;
	  int div;

  while(pos!=0)                   // LOGIC:
  {
    div=pos%2;                  // The logic used in this function is similar
				// to the process is used in the conventional
  bin += div*multi;             // long division method to find the binary.

  multi=multi*10;               // variable used:

   pos=pos/2 ;                  // multi: multiplier
 }                              // bin: Binary
			       // pos: decimal whos binary is to be found out.
 return bin;
  }



  int xcomp(int num)           // This is a function that is used in the
				// print().
    {
      int xcom;                // It divides num(binary) and obtains the last
				// two digits of the four digit binary.
       xcom=num%100;
	return xcom;         // Eg: if the binary is 1110, it will return the
			     // digits 10.

      }

   int ycomp(int num)         // This function is similar to xcomp()
     {
       int ycom;           // Just that instead of the last two digits,
			   // it returns the first two digits.
       ycom=num/100;
       return ycom;        // That is if the binary is 1110, it will return 11.

     }
				  // Note: All arrays are initialised to 50

  void voct(int n,int k[10][10],int r[10][10])   // A function to check the existance of a Verticle Octate in a K Map.
  {
   int i,j,octpos1=50,assign=0,pos[12],give=0,to[8],po=0;

   for(i=0;i<8;i++)                 // Variables Used:
     to[i]=50;
				    // octpos1 = stores the colomn where the
    for(i=0;i<12;i++)               //           verticle octate starts.
   {
     vo[i]=pos[i]=50;               // pos[]= if there is a case where more than
    }                               //      one V.Oct is present it saves the
       for(i=0;i<n-1;i++)           //      colunm no. of all the Octates.
       {
	 for(j=0;j<n-1;j++)         // to[]= temperory Octate, used just for
	 {                          //     printing purposes.


	  if(k[i][j]==1&&i==0&&k[i][j+1]==1)     // Check filter 1
	    {
	       for(int l=1;l<n;l++)
		{
			if(k[l][j]==1&&k[l][j+1]==1) // Check filter 2
			  {
			    octpos1=j;     // Octpos1 has the column no. from the Verticle octate starts.
			    }
			else
			{
			 octpos1=50;
			 break;
			}
		 }
		 if(octpos1!=50)
		 {
		 pos[assign]=octpos1;       // Octpos1 stored in the array 'pos' to make sure that in case of multiple octates, the postions are not lost.
		 assign++;
		 }
	      }

	   }
	  }
	  for(i=0;i<assign;i++)
	  {
	  if(pos[i]!=50)
	   {
		     for(int q=0;q<n;q++)
		     {
			for(int t=pos[i];t<=pos[i]+1;t++)
			      {                         //  Now, the positions of the Verticle Octate
			       to[po]=r[q][t];          //is fed into the array vo[] and to[].
				  po++;
			       vo[give]=r[q][t];
				       give++;
			       }
		     }

		  print(to,8);              // print funtion is called to print the output.
		po=0;                       // Note: As po is equated to 0 again, the array to[] is repeatedly overwritten.
		}
	     }
    }


  void hoct(int n,int k[10][10],int r[10][10])   //check for horizontal quad
  {

   int i,j,octpos=50,pos[12],assign=0,recieve=0,to[8],po=0;

   for(i=0;i<12;i++)            // Note: The process is similar to that of used in voct.
  ho[i]=pos[i]=50;

   for(i=0;i<n-1;i++)
    {
      for(j=0;j<n-1;j++)
	{
	 if(k[i][j]==1&&j==0&&k[i+1][j]==1)  // Filter 1
	  {
	     for(int l=0;l<n;l++)
	       {
			if(k[i][l]==1&&k[i+1][l]==1)   // Filter 2
			 {
			 octpos= i;           // The octpos is the position which stores the position of the row from which the horizontal Octate starts.
			  }

			else
			{
			 octpos=50;
			 break;
			 }
	       }

		 if(octpos!=50)
		 {
		 pos[assign]=octpos;        // If the K-Map consists of multiple horizontal Octates all the positions are saved in the array pos[]/
		 assign++;
		 }

	    }
	  }
	 }
	   for(i=0;i<assign;i++)
	  {

	  if(pos[i]!=50)
	   {
	     for(int q=pos[i];q<=pos[i]+1;q++)
	     {
	      for(int t=0;t<n;t++)
	      {
		to[po]=r[q][t];               // All the positions are saved in the array to(temporary Octate) and array ho(horizontal Octate)
		po++;
		ho[recieve]=r[q][t];
		recieve++;
	       }
	     }
		print(to,8);
		 po=0;                   // Note: As po is re-assigned to 0, the array to[] is constantly re- written and used for printing purposes.

	    }
	   }


	}

  void lvquad(int k[10][10],int n,int r[10][10])
  {
  // Among the variables used, vlpos[] and vo[] have been gloabally declared

   int i,j,vlquad=50,assign=0,tvlpos[4]={50,50,50,50},count=0,w;

   for(i=0;i<10;i++)
  vl[i]=vlpos[i]=50;

	  for(i=0;i<n;i++)
	   {
		for(j=0;j<n;j++)
		 {
		 if(k[i][j]==1)      // Filter 1
		    {
		     if(i==0)        // Filter 2
		      {
		      for(int l=0;l<n;l++)
		      {
			 if(k[l][j]==1)   // Filter 3
			   {
				  vlquad=j;  // The postion of the verticle quad is stored in vlquad
				   }
			else
			   {
				 vlquad=50;
				break;
			   }
	  }
	    if(vlquad!=50)
	    {
	      for(int u=0;u<n;u++)
	       {
		tvlpos[count]=r[u][vlquad];   // Now the positions of the verticle quad is saved in the tvlpos(Temporary Verticle Linear POSition ).
		 count++;
	       }

	     int a= check(tvlpos,vo,4,16);   // check() function is called. It will check whether 1st array(tvlpos) is a subset of the 2nd array passed (vo). If its a subset:0 is retuned; If not then it returns the value 1.

	     if(a==0)
	     {
	   for(int m=0;m<4;m++)
	    tvlpos[m]=50;           // If the V. Quad is a subset of the V. Oct then the V. Quad is again equated to 50

	     vlquad=50;
	      }

	      for(w=0;w<4;w++)
	      {
	      vl[assign]=tvlpos[w];
	       vlpos[assign]=tvlpos[w];    // In case of multiple V Quads in a K- map all the positions of the Quads are stored in vlpos.
	       assign++;
	       }
	    }

	print(tvlpos,4);
	count=0;

	}
       }
      }

    }

  }


  void lhquad(int k[10][10],int n,int r[10][10]) // Function to check the existance of Horizontal Linear Quad
   {
					       // The logic behind this function is similar to that of used in the funtion lvquad()
     int hlquad=50,i,j,l,assign=0,thlpos[4]={50,50,50,50},count=0;


     for(i=0;i<n;i++)
      {
	for(j=0;j<n;j++)
	{
	 if(k[i][j]==1)          // Filter 1
	  {
	   if(j==0)              //Filter 2; as we are looking for a horizontal quad, it must start from a position where j=0
	    {
	     for(int l=0;l<n;l++)
	      {
		if(k[i][l]==1)
		 {
		  hlquad=i;       // The Row where the horizotal quad exists is stored in hlquad.
		   }
	   else
	   {
	     hlquad=50;
	    break;
	    }
	  }
	    if(hlquad!=50)
	    {
	      for(int u=0;u<n;u++)
	       {
		thlpos[count]=r[hlquad][u]; // now the positions of the H Quad is fed into the array thlquad(Temporary Horizotal QUAD).
		 count++;
	      }

	     int a= check(thlpos,ho,4,16); // Check to remove redundancy if present.

	    if(a==0)                      // In case of a redundant group.
	    {
	      for(int t=0;t<4;t++)
	      thlpos[t]=50;

	    hlquad=50;
	     }

	     for(int w=0;w<4;w++)          // In case no redundancy, the position of the Quad is saved in
	     {
	      hl[assign]=thlpos[w];        // hl and hlpos(both globally declared)
	      hlpos[assign]=thlpos[w];

	      assign++;
	     }


	  }

	    print(thlpos,4);
	    count=0;                    // Note: thlpos is a array used only for printing and over-written every time a Horizontal Quad is found.
	}
       }
      }

    }

   }

 void quad(int k[10][10],int n,int r[10][10])
  {
     int i,j,flag[4]={50,50,50,50},l,m,assign=0;

   for(i=0;i<n-1;i++)
    {
      for(j=0;j<n-1;j++)
	{
	 if(k[i][j]==1)             // Filter 1
	  {
	   if(k[i][j+1]==1&&k[i+1][j]==1&&k[i+1][j+1]==1) // Filter 2
	   {
		flag[0]=r[i][j];
		flag[1]=r[i+1][j];        // The positions of the array now stored in the array flag.
		flag[2]=r[i][j+1];
		flag[3]=r[i+1][j+1];
	       }
	   int a= check(flag,vo,4,16);           // Check for redundancy

	     int b= check(flag,ho,4,16);

	  if(a==0||b==0)
	  {
	      for(int q=0;q<4;q++)
	       flag[q]=50;                 // If the group is redundant the array is equated to 50 again.
	   }

	 else                              // If the group is not redundant then the positions are saved in sqquadpos[],
	 {
	    for(int w=0;w<4;w++)
	   {
	   sqquadpos[assign]=flag[w];
	  assign++;
	    }
	  }
	    print(flag,4);            // printing the squad array.

	}
       }
     }
    }


  void vpair(int k[10][10],int n,int r[10][10])
  {
      int i,j,ppos[35],assign=0,a,b,c,d,flag[2]={50,50};  // ppos stands for : Pair POSition

      for(i=0;i<35;i++)
	ppos[i]=50;

      for(i=0;i<n-1;i++)
       {
	for(j=0;j<n;j++)
	{
		 if(k[i][j]==1&&k[i+1][j])    // Filter 1
		 {
		    flag[0]=r[i][j];         // Storing the positions of the pair in an array flag[].
		    flag[1]=r[i+1][j];

		a=check(flag,vo,2,16);
		b=check(flag,ho,2,16);          // Check for redundancy.
		c=check(flag,vl,2,8);
		d=check(flag,sqquadpos,2,8);

		 if(a==0||b==0||c==0||d==0)  // If flag is a subset of any of the array, a value of 50 is reassigned to it.
		 {
			 flag[0]=50;
			 flag[1]=50;
		 }
		  else
		  {
			for(int w=0;w<2;w++)     // if its not a subset of any array
			{
			 ppos[assign]=flag[w];   // the positions are saved in a permanent array namely, ppos[] as flag is repeatedly over-written.
			 assign++;
			}
		  }

	       print(flag,2);      // Printing of the pair.

	 }
       }

      }
   }


   void hpair(int k[10][10],int n,int r[10][10])  // To check for the existance of Horizontal pairs in a K Map.
  {
    int i,j,ppos[20],assign=0,a,b,c,d,flag[2]={50,50},count1;

      for(i=0;i<20;i++)
	ppos[i]=50;

       for(i=0;i<n;i++)
       {
	for(j=0;j<n-1;j++)
	{
		 if(k[i][j]==1&&k[i][j+1])   // Filter 1
		 {
		    flag[0]=r[i][j];
		    flag[1]=r[i][j+1];


		a=check(flag,vo,2,16);
		b=check(flag,ho,2,16);       // check for redundancy
		c=check(flag,hl,2,16);
		d=check(flag,sqquadpos,2,16);

       if(a==0||b==0||c==0||d==0)
       {
	 flag[0]=50;             // Re-assigning flag[] to 50 in case of of a redundant group.

	 flag[1]=50;
		}

       else                        //  If the pair is not redundant
       {
		for(int w=0;w<2;w++)
		{
		 ppos[assign]=flag[w];    // Storing the value of ppos[] if positions are redundant
		 assign++;

		 }
	}

    print(flag,2);

	  }

	 }

	}
    }

 int check(int a[20],int b[20],int n,int m)  // A function to remove Redundancy in a K Map.
  {
    int i,j,count=0,c[20];       // The function checks whether a[] is a subset of the array b[].
			      // If it is then it means that a[]  is redundant and must not be printed. In such a case a value of 0 is returned.
    for(i=0;i<20;i++)
    c[i]=0;

    for(i=0;i<n;i++)
    {

	for(j=0;j<m;j++)
	 {
	  if(a[i]==b[j])
	   {
	      c[i]=1;
	      break;
	   }
	 }
   }
   for(i=0;i<n;i++)
   {
    if(c[i]==1)
    {
     count++;
	  }
     }
    if(count!=n)
    return 1;                     //a[] is not a subset of b[].

   else
   return 0;                     //a[] is a subset of the b[].
 }

   int maproll(int k[10][10],int n,int r[10][10])  // Checking the extremes of the K Map for the existance of a Square Quad
   {

    int quadroll[4]={50,50,50,50},i,j,send=0;



   if(k[0][0]==1&&k[0][n-1]==1&&k[n-1][0]==1&&k[n-1][n-1]==1)
     {
       quadroll[0]=r[0][0];
       quadroll[1]=r[0][n-1];         //  This fuction checks for the following case
       quadroll[2]=r[n-1][0];

       quadroll[3]=r[n-1][n-1];       //  1  0  0  1
       send=1;                        //  0  0  0  0
      }                               //  0  0  0  0
      print(quadroll,4);              //  1  0  0  1

     return send;                 // The points in between may or may not have a high o/p

    }                        // If the aforemensioned case is satified, then 1 is returned else 0 is returned.



 void print(int t[10],int size)  // A function to print the output provided the positions of the member of the patter are stored in the array
 {
   int i,bin[10],xcm[10],ycm[10],a[10],b[10],c[10],d[10],acount=0,bcount=0,ccount=0,dcount=0,acount1=0,bcount1=0,ccount1=0,dcount1=0;

     for(i=0;i<size;i++)
     {
      if(t[i]!=50)              // LOGIC::
				//Lets assume a square quad having position as shown below have high o/p
      bin[i]=binary(t[i]);
			   // Position having high o/p are:
			   // {1,3,5,7}
      else
      bin[i]=50;
      }                     //  Its K Map will look like:

     for(i=0;i<size;i++)
     {                        //   0  1  1  0
     if(bin[i]!=50)           //   0  1  1  0
       xcm[i]=xcomp(bin[i]);  //   0  0  0  0
			      //   0  0  0  0
     else
       xcm[i]=50;
      }                  // The array t[] has the positions {1,3,7,5} stored in it.

    for(i=0;i<size;i++)  // another array bin[] is created with the binary values of {1,3,7,5} stored in it.
    {
    if(xcm[i]!=50)        // then the binary code is divided into two parts using xcomp() and ycomp() and stored in two seperate arrays.
	 c[i]=xcm[i]/10;
			  // These two arrays are further divided into two more arrays each with a single digit in them.
			  // Now in these arrays if all the elements are 1 or 0 then a undashed and dashed output maybe printed respectively.
			  // If these arrays are a combination of 1s and 0s not output is printed.
     else
      c[i]=50;
     }

			   // Eg:
			     //  t[]=  {1,3,5,7}

   for(i=0;i<size;i++)       //  bin[]={0001,0011,0101,0111}

   {                         //   xcm={00,00,01,01}      &  ycm={01,11,01,11}
    if(xcm[i]!=50)        // a[]={0,0,0,0} b[]={0,0,1,1}      c[]={0,1,0,1}      d[]={1,1,1,1}
   d[i]=xcm[i]%10;       //  a[]= All 0s     b[]= Mixed       c[]=Mixed    d[]= All 1s

   else                    // Therefore the Output is:  A'D.
   d[i]=50;
   }


   for(i=0;i<size;i++)
   {
	if(c[i]==1)
	 {
	   ccount++;
	   }

       if(c[i]==0)
       {
	ccount1++;
	}

       if(ccount==size)
       {
       cout<<"C";
       }

      if(ccount1==size)
      {
	cout<<"C'";
      }
    }

   for(i=0;i<size;i++)
   {
	if(d[i]==1)
	 {
	   dcount++;
	   }

       if(d[i]==0)
       {
	dcount1++;
	}

       if(dcount==size)
       {
       cout<<"D";
       }

      if(dcount1==size)
      {
	cout<<"D'";
      }
    }

     for(i=0;i<size;i++)
     {
       if(bin[i]!=50)
       ycm[i]=ycomp(bin[i]);

       else
       ycm[i]=50;
      }

    for(i=0;i<size;i++)
    {
      if(ycm[i]!=50)
       a[i]=ycm[i]/10;

      else
       a[i]=50;

     }

   for(i=0;i<size;i++)
   {
    if(ycm[i]!=50)
   b[i]=ycm[i]%10;

   else
    b[i]=50;
   }

   for(i=0;i<size;i++)
   {
	if(a[i]==1)
	 {
	   acount++;
	   }

       if(a[i]==0)
       {
	acount1++;
	}

       if(acount==size)
       {
       cout<<"A";
       }

      if(acount1==size)
      {
	cout<<"A'";
      }
    }

   for(i=0;i<size;i++)
   {
	if(b[i]==1)
	 {
	   bcount++;
	   }

       if(b[i]==0)
       {
	bcount1++;
	}

       if(bcount==size)
       {
       cout<<"B";
       }

      if(bcount1==size)
      {
	cout<<"B'";
      }
    }

    int flag=0;

   for(i=0;i<size;i++)
  {
  if(t[i]!=50)
   flag++;
   }
   if(flag==size)
   cout<<" + ";

  }

  void vmaproll(int k[10][10],int n,int r[10][10])   // Checks for Verticle pairs obtained by map rolling
  {
     int vroll[2],a,b;              // roll is global variable. It is used to check whether a K-Map has a case as illustrated in the function, maproll()

       if(roll!=1)                 // if the extreme corners of the map do not have a high o/p
      {
	for(int j=0;j<n;j++)         // Eg: The following is a case of verticle map rolling
	{
	 if(k[0][j]==1&&k[n-1][j]==1)
	 {                               //     0  0  1  0
	    vroll[0]=r[0][j];            //     0  0  0  0
	    vroll[1]=r[n-1][j];          //     0  0  0  0
					 //     0  0  1  0
	    a=check(vroll,vo,2,16);
	    b=check(vroll,vl,2,8);

	    if(a!=0&&b!=0)
		print(vroll,2);
	  }                              // In the above case, Positions {3,11} will form a verticle pair.
	 }
	}

     else
     {                                // If the extreme corners of the K Map are 1 then a quad is formed by map rolling. If that is the case, then further map rolling(horizontal and verticle) is not needed at those corners.

       for(int j=1;j<n-1;j++)        // Note: The checking here for rolling in this case starts from 1 and ends at n-2 as compared to the if condition where the checking goes from 0 all the way to n.
       {
	 if(k[0][j]==1&&k[n-1][j]==1)   // Eg:

	 {                              //  1  0  1  1
	    vroll[0]=r[0][j];           //  0  0  0  0
	    vroll[1]=r[n-1][j];         //  0  0  0  0
					//  1  0  1  1
	     print(vroll,2);

				  // In the case above, only the positions {3 and 11} should be taken and not {0,8} or {2,10} as the positions {0,2,8,10} collectively form a Square Quad.

			      // Sometimes when the map is rolled, we obtain Octates or Quads, but this is not performed in this code as they result in the same answer whether they are performed or not. For Example:



	}                   //  1  0  0  1         - In this case, a Octate will be formed by rolling entire columns 0 and 3.
      }                     //  1  0  0  1
    }                       //  1  0  0  1                     The equation of the Octate formed will be:  D'
			    //  1  0  0  1
   }                                           //   - However if it is taken as two verticle Quads the output will be:
					       //
					       //           C'D'+ CD'= (C'+C)D' =D'       Thus they give us the same result.





 void hmaproll(int k[10][10],int n,int r[10][10])
 {
				       // The logic is pretty much the same here as it was in vmaproll()
				       // Just that instead of verticle pairs we have horizontal pairs here.
 int hroll[2],a,b;

       if(roll!=1)                     // Again it is checked whether the K Map forms a Quad using Map Rolling.
       {
	for(int j=0;j<n;j++)            // Depending on that, the limits in the for-loop are decided.
	{
	 if(k[j][0]==1&&k[j][n-1]==1)
	 {
	    hroll[0]=r[j][0];
	    hroll[1]=r[j][n-1];

	 a= check(hroll,ho,2,16);
	 b= check(hroll,hl,2,16);
	  if(a!=0&&b!=0)
		print(hroll,2);

	  }

	 }

	}

     else
     {
       for(int j=1;j<n-1;j++)
       {

	 if(k[j][0]==1&&k[j][n-1]==1)
	 {
	    hroll[0]=r[j][0];
	    hroll[1]=r[j][n-1];

	     print(hroll,2);

	  }
	}
      }
    }


