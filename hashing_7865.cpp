/*  
	*****Programming Project(Prp)*****  
    *****NAME: Jaya Sathwika Gadi*****
	*****last four digits of NJIT ID: 7865*****
	*****UCID: jg899*****
	I HAVE READ THE COLLABORATION SECTION OF THE SYLLABUS. DOCUMENT 3 and PRP ADHERED
*/
#include "header.h"

class Lexicon_7865{
    public:
        int occupied; // Keys currently in this location
        int slots; // slots available which includes used and not used
        int lengthOfA; 
        int* T;
        char* A;
        int indexOfA;
        int maxLoopToInsert;

        Lexicon_7865(int slots){
            this->maxLoopToInsert = 0;
            this->occupied = 0;
            this->slots = slots;
            this->indexOfA = 0; 
            this->T = new int[this->slots];
            for(int i = 0; i < this->slots; i++){
                this->T[i] = -1;
            }
            this->lengthOfA = 15*this->slots; // Assuming an average word length of 14 or less (14 for word and 1 for the 0 byte separating the words)
            this->A = new char[this->lengthOfA];
        }
        ~Lexicon_7865(){
            delete this->T;
        }

        void doubleA(){
            char* newA = new char[2*this->lengthOfA];
            for(int i = 0; i < this->lengthOfA; i++){
                newA[i] = this->A[i];
            }
            this->lengthOfA = 2*this->lengthOfA;
            delete this->A;
            this->A = newA;
            return;
        }

        void doubleT(){
            this->slots *= 2;
            delete this->T;
            this->T = new int[this->slots];
            for(int i = 0; i < this->slots; i++){
                this->T[i] = -1;
            }
            this->occupied = 0;

            char* newA = new char[this->lengthOfA];
            for(int i = 0; i<this->lengthOfA; i++){
                newA[i] = this->A[i];
            }
            int max = this->indexOfA;
            this->indexOfA = 0;
            this->A = new char[this->lengthOfA];
            
            std::string wordSoFar = "";
            for(int i = 0; i<max; i++){
                if(newA[i] != '\0'){
                    wordSoFar += newA[i];
                }
                else{
                    int len = wordSoFar.length();
                    if(wordSoFar[0] == '*'){
                        int limit = this->indexOfA + len;
                        for(; this->indexOfA<limit; this->indexOfA++){
                            this->A[this->indexOfA] = '*';
                        }
                        this->A[this->indexOfA] = '\0';
                        this->indexOfA++;
                        wordSoFar = "";
                    }
                    else{
                        this->insert_hash(wordSoFar);
                        wordSoFar = "";
                    }
                }
            }
        }

/* ****** Insert ************ */
/*
// Function: insert_hash
// Description: Insert an entry into the Hash Table at a given index with a given "value"
// Parameters: string word
// Return Values: True or False
*/
        bool insert_hash(std::string word){
            if(this->search_hash(word) != -1){return(false);}
            while(1 + this->indexOfA + word.length() > this->lengthOfA || this->isFull()){this->doubleA(); this->doubleT();}
            for(int i = 0; i<this->slots; i++){
                int index = hashing(i, this->ascii_minus_four(word));
                this->maxLoopToInsert++;
                if(this->T[index] < 0){
                    // free to use to store
                    // actually do the storing
                    this->T[index] = this->indexOfA;
                    for(int j=0; j<word.length(); j++){
                        this->A[this->indexOfA] = word[j];
                        this->indexOfA++;
                    }
                    this->A[this->indexOfA] = '\0';
                    this->indexOfA++;
                    this->occupied++;
                    return(true);
                }
            }
            return(false);
        }

/* ****** Asciiminusfour ************ */
/*
// Function: ascii_minus_four
// Description: Substracting sum of ascii values with four
// Parameters: string word
// Return Values: sumMinusFour
*/
        int ascii_minus_four(std::string word){
            int sumMinusFour = 0;
            for(int i=0; i<word.length(); i++){
                sumMinusFour += word[i];
            }
            sumMinusFour -= 4;
            return(sumMinusFour);
        }

/* ********** Hashing ************ */
        int hashing(int i, int k){
            return(((i*i) + k)%this->slots);
        }

        std::string wordInA(int index){
            std::string rtn = "";
            for(int i = index; i<this->lengthOfA; i++){
                if(this->A[i] == '\0'){
                    break;
                }
                else{
                    rtn += this->A[i];
                }
            }
            return(rtn);
        }

/* ****** Search ************ */
/*
// Function: search_hash
// Description: Search the Hash Table for a word
// Parameters:  string word
// Return Values: The index of the element in the hash table or -1 if not found
*/
        int search_hash(std::string word){
            for(int i=0; i<this->maxLoopToInsert; i++){
                int index = this->hashing(i, this->ascii_minus_four(word));
                int Tindex = this->T[index];
                switch(Tindex){
                    // reached blank
                    case -1:
                        return(-1);
                    break;

                    // previously deleted entry
                    case -2:
                    break;

                    default:
                        std::string guess = this->wordInA(Tindex);
                        if(word.compare(guess) == 0){
                            return(index);
                        }
                        else if(guess.compare("") == 0 || guess[0] == '*'){
                            return(-1);
                        }
                    break;
                }
            }
            return(-1);
        }

/* ****** Delete ************ */
/*
// Function: delete_hash
// Description: Delete an entry in hash table T 
// Parameters: string word
// return values: None
*/
        int delete_hash(std::string word){
            int loc = this->search_hash(word);
            if(loc == -1){
                return(-1);
            }
            this->occupied--;
            for(int i = this->T[loc]; i<this->lengthOfA; i++){
                if(A[i] != '\0'){
                    A[i] = '*';
                }
                else{
                    this->T[loc] = -4;
                    return(loc);
                }
            }
            return(-1);
        }

/* ****** Print hash ************ */
/*
// Function: print_hash
// Description: Print the contents of the hash table (indexes of the first letter of a word in array A) and array A
*/
        std::string print_hash(){
            std::string rtn = " T\t\t\tA: ";
            for(int i=0; i<this->indexOfA; i++){
                if(A[i] == '\0'){
                    rtn += "\\";
                }
                else{
                    rtn += this->A[i];
                }
            }
            rtn += "\n";
            for(int i = 0; i < this->slots; i++){
                rtn += std::to_string(i) + ": ";
                rtn += this->T[i] >= 0 ? std::to_string(this->T[i]) : "";
                rtn += "\n";
            }
            return(rtn);
        }

/* ****** Empty hash ************ */
/*
// Function: isEmpty
// Description: Checks to see if a HashTable is empty
// Return Values: True (is empty), False (not empty)
*/
        bool isEmpty(){
            if(this->occupied==0)
            return(true);
            else
            return(false);
            }

/* ****** Full hash ************ */
/*
// Function: isFull
// Description: Determine if the Hash Table is full
// Return Values: True (full), False (empty)
*/
        bool isFull(){
            if(this->occupied >= this->slots)
            return(true);
            else
            return(false);
        }
};

class Hashbatch_7865{
    public:

        std::string filename;
        Lexicon_7865* hash_table;

        Hashbatch_7865(std::string filename){
            this->filename = filename;
        }

        void Create(int slots){
            hash_table = new Lexicon_7865(slots);
            return;
        }

        int search_hash(std::string word){
            return(hash_table->search_hash(word));
        }

        std::string print_hash(){
            return(hash_table->print_hash());
        }

        bool Empty(){
            return(hash_table->isEmpty());
        }

        bool Full(){
            return(hash_table->isFull());
        }
        
        int delete_hash(std::string word){
            return(hash_table->delete_hash(word));
        }

        bool insert_hash(std::string word){
            return(hash_table->insert_hash(word));
        }

/* ****** Batch ************ */
        void Batch(){
            std::string line;
            std::ifstream myfile(filename);
            if(myfile.is_open()){
                while(getline(myfile,line)){

                    std::string lexicon_created = line.substr(0, line.find(" "));

                    /* 
	Command 10 is Insert, 
	Command 11 is Deletion, 
	Command 12 is Search, 
	Command 13 is Print, 
	Command 14 is Create, 
	Command 15 is Comment
	*/
                    // Insert
                    if(lexicon_created.compare("10") == 0){
                        std::string next_word = line.substr(lexicon_created.length() + 1, line.length() - 1);
                        bool inserted = insert_hash(next_word);
                        if(!inserted){
                            int loc = search_hash(next_word);
                            std::cout << next_word + " ";
                            if(loc == -1){
                                std::cout << "\toverflow and cannot insert";
                            }
                            else{
                                std::cout << "\tfound at slot " + std::to_string(loc);
                            }
                            std::cout << "\n";
                        }
                    }

                    // Delete
                    else if(lexicon_created.compare("11") == 0){
                        std::string next_word = line.substr(lexicon_created.length() + 1, line.length() - 1);
                        int deletedIndex = delete_hash(next_word);
                        std::cout << next_word + " ";
                        if(deletedIndex == -1){
                            std::cout << "\tnot found";
                        }
                        else{
                            std::cout << "\tdeleted from slot " + std::to_string(deletedIndex);
                        }
                        std::cout << "\n";
                    }

                    // Search
                    else if(lexicon_created.compare("12") == 0){
                        std::string next_word = line.substr(lexicon_created.length() + 1, line.length() - 1);
                        int loc = search_hash(next_word);
                        std::cout << next_word + " ";
                        if(loc == -1){
                            std::cout << "\tnot found";
                        }
                        else{
                            std::cout << "\tfound at slot " + std::to_string(loc);
                        }
                        std::cout << "\n";
                    }

                    // Print
                    else if(lexicon_created.compare("13") == 0){
                        std::cout << print_hash();
                    }

                    // Create
                    else if(lexicon_created.compare("14") == 0){
                        std::string next_word = line.substr(lexicon_created.length() + 1, line.length() - 1);
                        int slots = stoi(next_word);
                        Create(slots);
                    }

                    // Comment
                    else if(lexicon_created.compare("15") == 0){
                        continue;
                    }
                }
                myfile.close();
            }
            return;
        }
};
/* ****** Main Function ************ */
int main(int argc, char* argv[]){
	if (argc <= 0)
		std::cout<< "Enter the correct format\n";
	else if (argc == 2){ 
		std::string filename = argv[1];
        Hashbatch_7865* inputfile = new Hashbatch_7865(filename);
        inputfile->Batch();
        delete inputfile;
	}
	else 
		std::cout<< "Expected format is ./filename textfile.txt\n";
	return(0);
}
