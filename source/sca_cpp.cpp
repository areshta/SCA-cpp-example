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
//*             No restrictions for copying or using
//*             Version 1.02 2017/04/29
//******************************************************************************  
//* History        Version 2.0  2018/06/20    - Template for error numbers

#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std; // issue 1 , line 31 : whole namespace is opened must be:
                     // using std::cout;
                     // using std::endl;
                     // using std::string;


class baseA
{
public:
    baseA() // issue 2 , line 40 : initialisation of members must be placed in initialisation list
    {
        int64_t* pB = new int64_t[10];     // issue 3 , line 42 : local pB hides member pB; pB member is not initialized
        pA = new int64_t[10];           // issue 4 , line 43 : Class 'baseA' does not have a copy constructor and operator= 
        pC = new int64_t;
        pD = new int64_t[10]; 
        int a = 7;                         // issue 5 , line 46 : using 'raw' int instead size specific int (as int32_t);
        cout << "Using 'raw' int " << a << endl;
        
        
    }                                     // issue 6 , line 50 : unused pB, memory leaks
    
    ~baseA()                            // issue 7 , line 52 : must be virtual ~baseA()
    {
        cout << "Destructor parent class baseA." << endl; 
        delete pA;                         // issue 8 , line 55 : must be 'delete[]';
        delete pB;                         // issue 9 , line 56 : 'delete' of non-initialized point64_ter
        delete[] pC;                     // issue 10 , line 57 : must be 'delete pC';
        
    } // issue 11 , line 59 : pD is not freed, memory leaks
    const char *getstr()
    {
        string a = "Ref to local variable" ;
        return a.c_str(); // issue 12 , line 63 : return the point64_ter that point64_ts inside local variable
    }

private:
    int64_t *pA;
    int64_t *pB;
    int64_t *pC;
    int64_t *pD; // issue 13 , line 70 : Class 'baseA' is unsafe, 'baseA::pD' can leak by wrong usage.
}; // issue 14 , line 71 : no user-provided copy constructor, no user-provided copy assignment operator


class childB: public baseA
{
public:

    childB() // issue 15 , line 78 : Member variable 'childB::a' is not initialized in the constructor
    {
        const char *str = "Some String"; // issue 16 , line 80 : using 'raw' char type ( could be wchar_t, char8_t, signed char...).
        pString = new char[strlen(str)]; // issue 17 , line 81 : Must be 'strlen(str)+1'
        (void)strcpy(pString,str);         // issue 18 , line 82 : using of strcpy is danger
        cout << "Non-initialized variable issue a = " << a << endl; // issue 19 , line 83 : Non-initialized variable using.
    } 
    void rec()
    {
        cout << "rec" << endl;
        rec();    // issue 20 , line 88 : recursion error
    }    
    void recA()
    {
        cout << "recA" << endl;
        recB(); // issue 21 , line 93 : cross recursion error
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
        return getA() / getB(); // issue 22 , line 110 : division by zero
    }   
    ~childB() // issue 23 , line 112 : destructor will never call, memory leaks
    {
        cout << "Destructor child class childB."<<endl; 
        delete pString; // issue 24 , line 115 : Must be delete[]
    }
private:
    char *pString;
    int64_t a;
}; // issue 25 , line 120 : no user-provided copy constructor, no user-provided copy assignment operator


class SimplyCreazy
{
public:
    SimplyCreazy(): pZero(0) {}    // issue 26 , line 126 : Member variable 'SimplyCreazy::pNonIni' is not initialized in the constructor
    
    void UnbrokenFor()
    {
        int a[10]; // issue 27 , line 130 : Variable 'a' is not assigned a value.
        for(int64_t i=0; i <10; i++)
        {
            if( i || !(42 < 0) )     // issue 28 , line 133 : no sense always true under if, 
            {
                i--; // issue 29 , line 135 : circle counter! as result the circle is unbroken
                cout << "a[i] = " << a[i] << endl;  // issue 30 , line 136 : access to non-init array, error 31: using signed index (size_t could be used)
                                                    // issue 31 , line 137 : using array where vector could be used
            }
        }
    }

    void BadPointerOperation()
    {
        
        strcpy(pZero,"Hello");         // issue 32 , line 145 : copy to zero point64_ter        
        strcpy(pNonIni,"Hello");     // issue 33 , line 146 : copy to non-initialized point64_ter
        *pZero++;                     // issue 34 , line 147 : non-using result of operation (* - no sense, unused), error 22: using nul point64_ter
        cout << "Creazy Point64_ters string: " << pZero << " " << pNonIni << endl; // error 22: using nul point64_ter
        return;
        // issue 35 , line 150 : bad control flow: unreachable instruction
        cout << "never accessable output " << endl;
    }
    
    void PassStringAsValue(string s) // issue 36 , line 154 : bad string transfer. Must be PassStringAsValue(const string& s)
    {
        cout << "Bad sending string example: " << s << endl; 
    }

private:
    char *pZero;
    char *pNonIni;
};

enum class Color {red, green, blue, magenta, yellow };

void switchBreak(Color cl)
{
    cout << "Lost break example begin" << endl;
    switch(cl)
    {
        case Color::red :
            cout << "Color::red" << endl;
            break;
        case Color::green : 
            cout << "Color::green. Ops! break is forgotten" << endl; // issue 37 , line 175 : missing break in the switch/case            
        case Color::blue : 
            cout << "Color::blue" << endl;
        case Color::magenta :
            [[gnu::fallthrow]]
        case Color::yellow :
            cout << "Color::yellow or Color::magenta" << endl;
    }
    cout << "Lost break example end" << endl;
}

int32_t  main (int32_t argc, const char* const argv[] ) 
{
    cout << "=== Happy start of the program :) ===" << endl;
    
    switchBreak(Color::green);

    int64_t crashCode = 0;
    if ( argc > 1 )
    {
        crashCode = atoi(argv[1]);
    }

    SimplyCreazy sc;

    baseA *pB = new childB(); //  issue 38 , line 200 : cause many errors 
    cout << "Error:"<< pB->getstr() << endl; 
    delete pB;

    if (crashCode > 6)
    {
        goto JUMP;         // issue 39 , line 206 : using goto
    }

    if (crashCode == 1 )
    {
        childB b1;
        childB b2(b1);  // issue 40 , line 212 : demo error of absence of deep copying
        b1 = b2;        // issue 41 , line 213 : demo error of absence of deep copying
    }

    else if (crashCode == 2 )
    {
        cout << "Unbroken recursion is starting..." << endl;
        childB b1;
        b1.rec();         // issue 42 , line 220 : recursion error
    }

    else if (crashCode == 3 )
    {
        cout << "Unbroken cross recursion is starting..." << endl;
        childB b1;
        b1.recA();         //  issue 43 , line 227 : cross recursion error
    }

    else if (crashCode == 4 )     // issue 44 , line 230 :  division by zero
    {
        cout << "Division by zero is starting..." << endl;
        int64_t c = childB::devZero() ;
        cout << "c = " << c << endl;
    }

    else if (crashCode == 5 )
    {
        sc.UnbrokenFor();             // issue 45 , line 239 : if with always true condition and unbroken circle 
    }
        
    else if (crashCode == 6 )
    {
        sc.BadPointerOperation();     // issue 46 , line 244 : cause many errors
    }                                // issue 47 , line 245 : finished else is recommended for else-if sequence

JUMP:
    
    string s("Some string");
    sc.PassStringAsValue(s);         // issue 48 , line 250 : uncorrect string sending

    cout << "=== Happy end of the program :) number of issues inside is about 49 :) ===" << endl;

    return 0;
}


