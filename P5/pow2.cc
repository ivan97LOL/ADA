//Iván Álvarez García 49623492A
#include <iostream>
#include "math.h"

using namespace std;
long PASOS = 0;

//coste: \Theta(n)
unsigned long pow2_1(unsigned n){
    unsigned long pow = 1;
    for(unsigned i = 0; i<n ; i++){
        pow *= 2;
        PASOS++;
    }
    
    return pow;
}

//coste: \Theta(log(n))
unsigned long pow2_2(unsigned n){
    if(n == 0){
        PASOS++;
        return 1;
    }
    PASOS++;
    unsigned long pow = pow2_2(n-1);
    return pow*2;
}

//coste: \Theta(1)
unsigned long pow2_3(unsigned n){
    PASOS++;
    return (1 << n);
}

int main(){

    unsigned n = 5;
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

            cout<<"\t" << n << "\t" << std::flush; 

            pow2_1(n);

            cout <<"\t"<< PASOS <<"\t"<< std::flush;

            PASOS = 0;
            pow2_2(n);

            cout <<"\t"<< PASOS <<"\t"<< std::flush;

            PASOS = 0;
            pow2_3(n);

            cout <<"\t"<< PASOS << "\t"<<std::flush;
            cout<<endl;

            n += 2;
        }

        
    }
}