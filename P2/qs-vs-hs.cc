//Iván Álvarez García DNI: 49623492A
/*
ADA. 2022-23
Práctica 2:"Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
*/
#include <unistd.h>
#include <iostream>
#include <math.h>


using namespace std;

double STEPS = 0;

//--------------------------------------------------------------
// Middle Quick Sort
void middle_QuickSort(int * v, long left, long right){
    STEPS++;
	if (left<right){
	    long i=left; 
        long j=right;
		int pivot=v[(i+j)/2];
		do {
            while (v[i]<pivot) {i++; STEPS++;}
            while (v[j]>pivot) {j--; STEPS++;}
            if (i<=j){ 
                swap(v[i], v[j]);
                i++; j--;
            }
        } while (i<=j);
        if (left < j)  {middle_QuickSort(v,left,j);}
        if (i < right) {middle_QuickSort(v,i,right);}
    }
    STEPS++;
}

//--------------------------------------------------------------                
//HEAPSORT
// Procedure sink used by Heapsort algorith
// Sink an element (indexed by i) in a tree to keep the heap property. n is the size of the heap. 

void sink(int *v, size_t n, size_t i) {

    STEPS++;
    do {
        size_t largest = i;  // Initialize largest as root
        size_t l = 2*i + 1;  // left = 2*i + 1
        size_t r = 2*i + 2;  // right = 2*i + 2
 
    // Is left child larger than root?
        if (l < n && v[l] > v[largest]){
            largest = l;
        }
 
    // Is right child larger than largest so far
        if (r < n && v[r] > v[largest]){
            largest = r;
        }
 
    // If largest is still root then the process is done
        if (largest == i){
            break;
        }
    
	// If not, swap new largest with current node i and repeat the process with childs. 
        swap(v[i], v[largest]);
        STEPS++;
        i=largest;  
     } while (true);
}
 
//--------------------------------------------------------------                
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n) {
    STEPS++;
    if( n < 2 ){ // nothing to do
        return;
    }

    // Build a MAX-HEAP with the input array
    for (size_t i = n / 2 - 1; true; i--) {
        sink(v, n, i);
        STEPS++;
        if (i==0){
            break;
        } //as size_t is unsigned type
	}	

  	// A this point we have a HEAP on the input array, let's take advantage of this to sort the array:
    // One by swap the first element, which is the largest (max-heap), with the last element of the vector and rebuild heap by sinking the new placed element on the begin of vector.  
    for (size_t i=n-1; i>0; i--) {
		// Move current root to the end.
        swap(v[0], v[i]);
 		// Now largest element is at the end but do not know if the element just moved to the beginning is well placed, so a sink process is required.
		// Note that, at the same time, the HEAP size is reduced one element by one.
        sink(v, i, 0);
        STEPS++;
		// The procedure ends when HEAP has only one element, the lower of the input array
    }
}


int main(void){

    srand(0);

    cout << "#QUICKSORT VERSUS HEAPSORT."
        << endl
        << "#Average processing Msteps (millions of program steps)"
        << endl;
    cout <<"#Number of samples (arrays of integer): 30"<<endl;
    cout << endl;

        cout<<"#                    RANDOM ARRAYS              SORTED ARRAYS       REVERSE SORTED ARRAYS"<<endl;
        cout<<"#                  -------------------       -------------------    ---------------------"<<endl;
        cout<<"#    Size          QuickSort  HeapSort       QuickSort  HeapSort    QuickSort  HeapSort"<<endl;
        cout<<"#========================================================================================"<<endl;

    for (int exp = 15; exp <= 20; exp++){
        size_t size = size_t( pow(2,exp) );
        int* v_random = new int [size];
        int* v_sorted = new int [size];
        int* v_reverse_sorted = new int [size];

        if (!v_random || !v_sorted || !v_reverse_sorted){
            cerr << "Error, not enough memory!" << endl;
            exit (EXIT_FAILURE);  
        }

        cout<<"     " << size << "        " << std::flush; 
        
        for(int i = 0; i<30; i++){
           //steps in v_random qs
           for (size_t j = 0; j < size; j++) 
                v_random[j] = rand();
            middle_QuickSort(v_random,0,size-1);
        }

        cout <<"  "<< (STEPS / 1000000.0)/30 << std::flush;
        cout << "\t";
        
        STEPS = 0;

        for(int i = 0; i<30; i++){
           //steps in v_random hs
           for (size_t j = 0; j < size; j++) 
                v_random[j] = rand();

            heapSort(v_random, size);
        }

        cout <<"  "<< (STEPS / 1000000.0)/30 << std::flush;
        cout << "  ";
        STEPS = 0;

        for(size_t i = 0; i < size; i++)
            v_sorted[i] = i;

        for(size_t i = 0; i < size; i++)
            v_reverse_sorted[i] = (size-1)-i;

        //steps in v_ordered
        middle_QuickSort(v_sorted,0,size-1);

        cout <<"  "<< (STEPS / 1000000.0) << std::flush;
        cout << "  ";
        
        STEPS = 0;

        heapSort(v_sorted,size);

        cout <<"  "<< (STEPS / 1000000.0) << std::flush;
        cout << "  ";
        
        STEPS = 0;
        
        //steps in v_reverted
        middle_QuickSort(v_reverse_sorted,0,size-1);

        cout <<"  "<< (STEPS / 1000000.0) << std::flush;
        cout << "  ";
        
        STEPS = 0;

        for(size_t i = 0; i > 0; i++)
            v_reverse_sorted[i] = (size-1)-i;

        heapSort(v_reverse_sorted,size);

        cout <<"  "<< (STEPS / 1000000.0) << std::flush;
        cout << endl;
        
        STEPS = 0;

        for (size_t i = 1; i < size; i++)
            if (v_random[i] < v_random[i-1]){ 
                cerr << "Panic, array not sorted! " << i << endl; 
                exit(EXIT_FAILURE);            
            }

        delete[] v_random; 

        for (size_t i = 1; i < size; i++)
            if (v_sorted[i] < v_sorted[i-1]){ 
                cerr << "Panic, array not sorted! " << i << endl; 
                exit(EXIT_FAILURE);            
            }
        delete[] v_sorted;

        for (size_t i = 1; i < size; i++)
            if (v_reverse_sorted[i] < v_reverse_sorted[i-1]){ 
                cerr << "Panic, array not sorted! " << i << endl; 
                exit(EXIT_FAILURE);            
            }
        delete[] v_reverse_sorted;
    }
}