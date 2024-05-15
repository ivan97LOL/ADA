//Iván Álvarez García 49623492A
#include <iostream>
#include "math.h"

using namespace std;
long PASOS = 0;

//coste: \Theta(2^n)
unsigned long pow2_1(unsigned n){
    if(n == 0){
        PASOS++;
        return 1;
    }
    PASOS++;
    return pow2_1(n-1) + pow2_1(n-1);
}

//coste: \Theta(n)
unsigned long pow2_2(unsigned n){
    unsigned long pow = 1;
    for(unsigned i = 0; i<n ; i++){
        pow *= 2;
        PASOS++;
    }
    
    return pow;
}

//coste: \Theta(log(n))
unsigned long pow2_3(unsigned n){
    if(n == 0){
        PASOS++;
        return 1;
    }
    PASOS++;
    unsigned long pow = pow2_3(n/2);
    if(n%2 == 0){
        return pow*pow;
    }
    else{
        return 2*pow*pow;
    }
}

int main(){

    unsigned n = 2;
    cout << "#POW OF 2 WITH DIFFERENT METHODS."
         << endl;
    cout <<"#Number of samples: 10"<<endl;
    cout << endl;

        cout<<"#        N             POW2_1           POW2_2       POW2_3"<<endl;
        cout<<"#                      ------           ------       ------"<<endl;
        cout<<"#===================================================================="<<endl;

    for(int i = 0; i<10; i++){
        if(pow2_1(n) != pow(2,n)){
            cout<< "INCORRECTO POW2_1()"<<endl;

        }else if(pow2_2(n) != pow(2,n)){
            cout<< "INCORRECTO POW2_2()"<<endl;
        }else if(pow2_3(n)!=pow(2,n)){
            cout<< "INCORRECTO POW2_3()"<<endl;
        }else{
            cout<<"\t" <<"\t" << n << "\t" <<"\t" << std::flush; 
            pow2_1(n);
            cout <<"\t" <<"\t"<< PASOS <<"\t" <<"\t" << std::flush;
            PASOS = 0;
            pow2_2(n);
            cout <<"\t" <<"\t" << PASOS <<"\t" <<"\t" << std::flush;
            PASOS = 0;
            pow2_3(n);
            cout <<"\t" <<"\t" << PASOS << "\t" <<"\t" <<std::flush;
            cout<<endl;

            n += 1;
        }        
    }
}