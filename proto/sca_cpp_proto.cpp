//******************************************************************************
//* Static Code C++ Analyzers testing example
//*
//* Warning: This example is not the way to find out what analyzer "is better".
//*          It consists few issues only. There are thousands code issues.
//*          If you create other example with other set of errors you could have other results of comparing.
//*          Results depends from analyzers options too          
//*	
//* Purpose: The purpose of this example is draft verifying of analyzer settings and demonstration the fact
//*          that any analyzer does not cover all issues in the code that can be found during static analyses 
//*
//* Technical notes:
//*           - For looking false negative analyzer issues only
//*           - The examples includes 38 genera c++ issues of different priority, 6 of them are catastrophic
//*           - The example does not include stl issues and c++11/14/17 specific issues
//*           - The example does not include multi-thread issues
//*           - The example does not include cross-modules issues
//*           - A lot of of style and non-serious issues is uncounted
//*           - gcc 5.4.0 compiles it without warnings (if used without parameters)
//*
//* Created by: Alex Reshta. 
//* 			No restrictions for copying or using
//* 			Version 1.02 2017/04/29
//******************************************************************************  
//* History		Version 2.0  2018/06/20	- Template for error numbers

#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std; // issue {autonum} , line {line} : whole namespace is opened must be:
					 // using std::cout;
					 // using std::endl;
					 // using std::string;


class baseA
{
public:
    baseA() // issue {autonum} , line {line} : initialisation of members must be placed in initialisation list
	{
		int64_t* pB = new int64_t[10]; 	// issue {autonum} , line {line} : local pB hides member pB; pB member is not initialized
        pA = new int64_t[10];           // issue {autonum} , line {line} : Class 'baseA' does not have a copy constructor and operator= 
        pC = new int64_t;
		pD = new int64_t[10]; 
		int a = 7; 						// issue {autonum} , line {line} : using 'raw' int instead size specific int (as int32_t);
        cout << "Using 'raw' int " << a << endl;
		
		
    } 									// issue {autonum} , line {line} : unused pB, memory leaks
    
    ~baseA()							// issue {autonum} , line {line} : must be virtual ~baseA()
    {
		cout << "Destructor parent class baseA." << endl; 
		delete pA; 						// issue {autonum} , line {line} : must be 'delete[]';
		delete pB; 						// issue {autonum} , line {line} : 'delete' of non-initialized point64_ter
		delete[] pC; 					// issue {autonum} , line {line} : must be 'delete pC';
		
    } // issue {autonum} , line {line} : pD is not freed, memory leaks
	const char *getstr()
	{
		string a = "Ref to local variable" ;
		return a.c_str(); // issue {autonum} , line {line} : return the point64_ter that point64_ts inside local variable
    }

private:
    int64_t *pA;
	int64_t *pB;
    int64_t *pC;
    int64_t *pD; // issue {autonum} , line {line} : Class 'baseA' is unsafe, 'baseA::pD' can leak by wrong usage.
}; // issue {autonum} , line {line} : no user-provided copy constructor, no user-provided copy assignment operator


class childB: public baseA
{
public:

    childB() // issue {autonum} , line {line} : Member variable 'childB::a' is not initialized in the constructor
    {
	    const char *str = "Some String"; // issue {autonum} , line {line} : using 'raw' char type ( could be wchar_t, char8_t, signed char...).
	    pString = new char[strlen(str)]; // issue {autonum} , line {line} : Must be 'strlen(str)+1'
	    (void)strcpy(pString,str);		 // issue {autonum} , line {line} : using of strcpy is danger
        cout << "Non-initialized variable issue a = " << a << endl; // issue {autonum} , line {line} : Non-initialized variable using.
    } 
    void rec()
    {
	    cout << "rec" << endl;
	    rec();	// issue {autonum} , line {line} : recursion error
    }    
    void recA()
    {
		cout << "recA" << endl;
		recB(); // issue {autonum} , line {line} : cross recursion error
    }
    void recB()
    {
		cout << "recB" << endl;
		recA();
    }		
    static int64_t getA()
    {
	    return 1;
    }
    static int64_t getB()
    {
	    return 0;
    }
    static int64_t devZero()
    {
	    return getA() / getB(); // issue {autonum} , line {line} : division by zero
    }   
	~childB() // issue {autonum} , line {line} : destructor will never call, memory leaks
	{
		cout << "Destructor child class childB."<<endl; 
		delete pString; // issue {autonum} , line {line} : Must be delete[]
    }
private:
	char *pString;
	int64_t a;
}; // issue {autonum} , line {line} : no user-provided copy constructor, no user-provided copy assignment operator



class SimplyCreazy
{
public:
	SimplyCreazy(): pZero(0) {}	// issue {autonum} , line {line} : Member variable 'SimplyCreazy::pNonIni' is not initialized in the constructor
	
	void UnbrokenFor()
	{
		int a[10]; // issue {autonum} , line {line} : Variable 'a' is not assigned a value.
		for(int64_t i=0; i <10; i++)
		{
			if( i || !(42 < 0) ) 	// issue {autonum} , line {line} : no sense always true under if, 
			{
				i--; // issue {autonum} , line {line} : circle counter! as result the circle is unbroken
				cout << "a[i] = " << a[i] << endl;  // issue {autonum} , line {line} : access to non-init array, error 31: using signed index (size_t could be used)
													// issue {autonum} , line {line} : using array where vector could be used
			}
		}
	}

	void BadPointerOperation()
	{
		
		strcpy(pZero,"Hello"); 		// issue {autonum} , line {line} : copy to zero point64_ter		
		strcpy(pNonIni,"Hello"); 	// issue {autonum} , line {line} : copy to non-initialized point64_ter
		*pZero++; 					// issue {autonum} , line {line} : non-using result of operation (* - no sense, unused), error 22: using nul point64_ter
		cout << "Creazy Point64_ters string: " << pZero << " " << pNonIni << endl; // error 22: using nul point64_ter
		return;
		// issue {autonum} , line {line} : bad control flow: unreachable instruction
		cout << "never accessable output " << endl;
	}
	
	void PassStringAsValue(string s) // issue {autonum} , line {line} : bad string transfer. Must be PassStringAsValue(const string& s)
	{
	    cout << "Bad sending string example: " << s << endl; 
	}

private:
	char *pZero;
	char *pNonIni;
};


int32_t  main (int32_t argc, const char* const argv[] ) 
{
    cout << "=== Happy start of the program :) ===" << endl;

    int64_t crashCode = 0;
    if ( argc > 1 )
    {
	    crashCode = atoi(argv[1]);
    }

    SimplyCreazy sc;

    baseA *pB = new childB(); //  issue {autonum} , line {line} : cause many errors 
    cout << "Error:"<< pB->getstr() << endl; 
    delete pB;

    if (crashCode > 6)
    {
	    goto JUMP; 		// issue {autonum} , line {line} : using goto
    }

    if (crashCode == 1 )
    {
	    childB b1;
	    childB b2(b1);  // issue {autonum} , line {line} : demo error of absence of deep copying
	    b1 = b2;		// issue {autonum} , line {line} : demo error of absence of deep copying
    }

    else if (crashCode == 2 )
    {
	    cout << "Unbroken recursion is starting..." << endl;
	    childB b1;
	    b1.rec(); 		// issue {autonum} , line {line} : recursion error
    }

    else if (crashCode == 3 )
    {
	    cout << "Unbroken cross recursion is starting..." << endl;
	    childB b1;
	    b1.recA(); 		//  issue {autonum} , line {line} : cross recursion error
    }

    else if (crashCode == 4 ) 	// issue {autonum} , line {line} :  division by zero
    {
	    cout << "Division by zero is starting..." << endl;
	    int64_t c = childB::devZero() ;
	    cout << "c = " << c << endl;
    }

    else if (crashCode == 5 )
    {
	    sc.UnbrokenFor(); 			// issue {autonum} , line {line} : if with always true condition and unbroken circle 
    }
	    
    else if (crashCode == 6 )
    {
	    sc.BadPointerOperation(); 	// issue {autonum} , line {line} : cause many errors
    }								// issue {autonum} , line {line} : finished else is recommended for else-if sequence

JUMP:
	
	string s("Some string");
	sc.PassStringAsValue(s); 		// issue {autonum} , line {line} : uncorrect string sending

    cout << "=== Happy end of the program :) number of issues inside is about {number_issues} :) ===" << endl;

	return 0;
}


