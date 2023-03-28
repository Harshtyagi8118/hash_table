
/*************************************************************/


/* */


/* Starter file for a2 */


/* */


/* Author1 Name: <name here> */


/* - class/function list/main author or main checker */


/* Author2 Name: <name here> */


/* - class/function list/main author or main checker */


/* */


/*************************************************************/




#include <string>


#include <utility>


#include <functional>

#include "timer.h"
// #include "mylist.h"
#include <vector>
#include <list>



template <class TYPE>


class Table{


public:


Table(){}

double loadFactor() const ;
virtual bool update(const std::string& key, const TYPE& value)=0;


virtual bool remove(const std::string& key)=0;


virtual bool find(const std::string& key, TYPE& value)=0;


virtual int numRecords() const = 0;


virtual bool isEmpty() const = 0;


virtual int capacity() const = 0;


virtual ~Table(){}


};





template <class TYPE>


class SimpleTable:public Table<TYPE>{





struct Record{


TYPE data_;


std::string key_;


Record(const std::string& key, const TYPE& data){


key_=key;


data_=data;


}





};





Record** records_; //the table


int capacity_; //capacity of the array



public:


SimpleTable(int capacity);


SimpleTable(const SimpleTable& rhs);


SimpleTable(SimpleTable&& rhs);


virtual bool update(const std::string& key, const TYPE& value);


virtual bool remove(const std::string& key);


virtual bool find(const std::string& key, TYPE& value);


virtual const SimpleTable& operator=(const SimpleTable& rhs);


virtual const SimpleTable& operator=(SimpleTable&& rhs);


virtual ~SimpleTable();


virtual bool isEmpty() const{return numRecords()==0;}


virtual int numRecords() const;


virtual int capacity() const {return capacity_;}





};


template <class TYPE>


int SimpleTable<TYPE>::numRecords() const{


int rc=0;


for(int i=0;records_[i]!=nullptr;i++){


rc++;


}


return rc;


}




template <class TYPE>


SimpleTable<TYPE>::SimpleTable(int capacity): Table<TYPE>(){


records_=new Record*[capacity+1];


capacity_=capacity;


for(int i=0;i<capacity_+1;i++){


records_[i]=nullptr;


}


}





template <class TYPE>


SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& rhs){


records_=new Record*[rhs.capacity_+1];


capacity_=rhs.capacity_;


for(int i=0;i<capacity_+1;i++){


records_[i]=nullptr;


}


for(int i=0;i<rhs.numRecords();i++){


update(rhs.records_[i]->key_,rhs.records_[i]->data_);


}


}


template <class TYPE>


SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& rhs){


capacity_=rhs.capacity_;


records_=rhs.records_;


rhs.records_=nullptr;


rhs.capacity_=0;


}





template <class TYPE>


bool SimpleTable<TYPE>::update(const std::string& key, const TYPE& value){


int idx=-1;


int sz=numRecords();


bool rc=true;


for(int i=0;i<sz;i++){


if(records_[i]->key_ == key){


idx=i;


}


}


if(idx==-1){


if(sz < capacity_){


records_[numRecords()]=new Record(key,value);


for(int i=numRecords()-1;i>0 && records_[i]->key_ < records_[i-1]->key_;i--){


Record* tmp=records_[i];


records_[i]=records_[i-1];


records_[i-1]=tmp;


}


}


else{


rc=false;


}


}


else{


records_[idx]->data_=value;


}


return rc;


}





template <class TYPE>


bool SimpleTable<TYPE>::remove(const std::string& key){


int idx=-1;


for(int i=0;records_[i]!=nullptr;i++){


if(records_[i]->key_ == key){


idx=i;


}


}


if(idx!=-1){


delete records_[idx];





for(int i=idx;i<numRecords()-1;i++){


records_[i]=records_[i+1];


}

int size = numRecords();
records_[size-1]=nullptr;


return true;


}


else{


return false;


}


}





template <class TYPE>


bool SimpleTable<TYPE>::find(const std::string& key, TYPE& value){


int idx=-1;


for(int i=0;i<numRecords();i++){


if(records_[i]->key_ == key){


idx=i;


}


}


if(idx==-1)


return false;


else{


value=records_[idx]->data_;


return true;


}


}





template <class TYPE>


const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& rhs){


if(this!=&rhs){


if(records_){


while(numRecords() != 0){


remove(records_[0]->key_);


}


delete [] records_;


}


records_=new Record*[rhs.capacity_+1];


capacity_=rhs.capacity_;


for(int i=0;i<capacity_;i++){


records_[i]=nullptr;


}


for(int i=0;i<rhs.numRecords();i++){


update(rhs.records_[i]->key_,rhs.records_[i]->data_);


}





}


return *this;


}


template <class TYPE>


const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& rhs){


if(records_){


while(numRecords() != 0){


remove(records_[0]->key_);


}


delete [] records_;


}


records_=rhs.records_;


capacity_=rhs.capacity_;


rhs.records_=nullptr;


rhs.capacity_=0;





return *this;


}

// template <class TYPE>
// double SimpleTable<TYPE>::loadFactor() const {
//   return static_cast<double>(records_) / static_cast<double>(capacity_);
// }





template <class TYPE>


SimpleTable<TYPE>::~SimpleTable(){


if(records_){


int sz=numRecords();


for(int i=0;i<sz;i++){


remove(records_[0]->key_);


}


delete [] records_;


}


}


template <class TYPE>


class ChainingTable:public Table<TYPE>{

private:
    int m_capacity;
    std::vector<std::pair<std::string, TYPE>>* m_table;
    int m_numRecords;

    int hash(const std::string& key) const {
        int hashVal = 0;
        for (char ch : key) {
            hashVal = 37 * hashVal + ch;
        }
        return hashVal % m_capacity;
    }
public:


ChainingTable(int maxExpected);


ChainingTable(const ChainingTable& other);


ChainingTable(ChainingTable&& other);


double loadFactor() const;


virtual bool update(const std::string& key, const TYPE& value);


virtual bool remove(const std::string& key);


virtual bool find(const std::string& key, TYPE& value);


virtual const ChainingTable& operator=(const ChainingTable& other);


virtual const ChainingTable& operator=(ChainingTable&& other);


virtual ~ChainingTable();


virtual bool isEmpty() const;


virtual int numRecords() const;


virtual int capacity() const;


};





template <class TYPE>


ChainingTable<TYPE>::ChainingTable(int capacity): Table<TYPE>() , m_capacity(capacity * 2), m_numRecords(0) {
    m_table = new std::vector<std::pair<std::string, TYPE>>[m_capacity];
}





template <class TYPE>

ChainingTable<TYPE>::ChainingTable(const ChainingTable<TYPE>& other): Table<TYPE>(), m_capacity(other.m_capacity), m_numRecords(other.m_numRecords) {
    m_table = new std::vector<std::pair<std::string, TYPE>>[m_capacity];
    for (int i = 0; i < m_capacity; i++) {
        m_table[i] = other.m_table[i];
    }
}


template <class TYPE>

ChainingTable<TYPE>::ChainingTable(ChainingTable<TYPE>&& other): Table<TYPE>(), m_capacity(other.m_capacity), m_table(other.m_table), m_numRecords(other.m_numRecords) {
    other.m_capacity = 0;
    other.m_table = nullptr;
    other.m_numRecords = 0;
}


template <class TYPE>


bool ChainingTable<TYPE>::update(const std::string& key, const TYPE& value){

int index = hash(key);
    for (auto& entry : m_table[index]) {
        if (entry.first == key) {
            entry.second = value;
            return true;
        }
    }
    m_table[index].push_back(std::make_pair(key, value));
    ++m_numRecords;
    return true;

}





template <class TYPE>


bool ChainingTable<TYPE>::remove(const std::string& key){

 int index = hash(key);
    auto& chain = m_table[index];
    for (auto itr = chain.begin(); itr != chain.end(); ++itr) {
        if (itr->first == key) {
            chain.erase(itr);
            --m_numRecords;
            return true;
        }
    }
    return false;
// return true;


}





template <class TYPE>


bool ChainingTable<TYPE>::find(const std::string& key, TYPE& value){
   int index = hash(key);
    for (auto& entry : m_table[index]) {
        if (entry.first == key) {
            value = entry.second;
            return true;
        }
    }
    return false;


}





template <class TYPE>


const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(const ChainingTable<TYPE>& other){

 if (this != &other) {
        m_capacity = other.m_capacity;
        m_numRecords = other.m_numRecords;
        delete[] m_table;
        m_table = new std::vector<std::pair<std::string, TYPE>>[m_capacity];
        for (int i = 0; i < m_capacity; ++i) {
            m_table[i] = other.m_table[i];
        }
    }
    return *this;
}


template <class TYPE>

const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(ChainingTable<TYPE>&& other){

   if (this != &other) {
        m_capacity = other.m_capacity;
        m_numRecords = other.m_numRecords;
        m_table = other.m_table;
        other.m_table = nullptr;
    }
    return *this;

}





template <class TYPE>


ChainingTable<TYPE>::~ChainingTable(){

delete[] m_table;


}





template <class TYPE>


bool ChainingTable<TYPE>::isEmpty() const {

return m_numRecords == 0;
// return false;


}





template <class TYPE>


int ChainingTable<TYPE>::numRecords() const {

return m_numRecords;
// return 0;


}


template <class TYPE>


int ChainingTable<TYPE>::capacity() const {

return m_capacity;

}





template <class TYPE>


double ChainingTable<TYPE>::loadFactor() const {
  return static_cast<double>(m_numRecords) / static_cast<double>(m_capacity);
}











template <class TYPE>


class LPTable:public Table<TYPE>{


public:


LPTable(int maxExpected);


LPTable(const LPTable& other);


LPTable(LPTable&& other);


double loadFactor() const;


virtual bool update(const std::string& key, const TYPE& value);


virtual bool remove(const std::string& key);


virtual bool find(const std::string& key, TYPE& value);


virtual const LPTable& operator=(const LPTable& other);


virtual const LPTable& operator=(LPTable&& other);


virtual ~LPTable();


virtual bool isEmpty() const;


virtual int numRecords() const;


virtual int capacity() const;

private:

struct Record {
std::string key;
TYPE value;
bool inUse;
};


static const double MAX_LOAD_FACTOR;
int capacity_;
int size_;
Record* records_;

int hash(const std::string& key) const {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c) % capacity_;
    }
    return hash;
}


};
template <class TYPE>
const double LPTable<TYPE>::MAX_LOAD_FACTOR = 0.7;



template <class TYPE>


LPTable<TYPE>::LPTable(int capacity): Table<TYPE>(),capacity_(capacity), size_(0) {
        records_ = new Record[capacity_];
        for (int i = 0; i < capacity_; i++) {
            records_[i].inUse = false;
        }

}





template <class TYPE>


LPTable<TYPE>::LPTable(const LPTable<TYPE>& other): capacity_(other.capacity_), size_(other.size_) {
        records_ = new Record[capacity_];
        for (int i = 0; i < capacity_; i++) {
            records_[i] = other.records_[i];
        }
}


template <class TYPE>


LPTable<TYPE>::LPTable(LPTable<TYPE>&& other):capacity_(other.capacity_), size_(other.size_), records_(other.records_) {
        other.records_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
}


template <class TYPE>


bool LPTable<TYPE>::update(const std::string& key, const TYPE& value) {

if (loadFactor() >= MAX_LOAD_FACTOR) {
            return false; // table is full
        }
        int index = hash(key);
        int i = 0;
        while (i < capacity_) {
            if (!records_[index].inUse) {
                // empty slot found, add record here
                records_[index].inUse = true;
                records_[index].key = key;
                records_[index].value = value;
                size_++;
                return true;
            }
            if (records_[index].key == key) {
                // key already exists, update the value
                records_[index].value = value;
                return true;
            }
            // collision detected, try next slot
            index = (index + 1) % capacity_;
            i++;
        }
        return false; 



}





template <class TYPE>


bool LPTable<TYPE>::remove(const std::string& key){

int index = hash(key);
        int i = 0;
        while (i < capacity_) {
            if (records_[index].inUse && records_[index].key == key) {
                // key found, mark record as not in use
                records_[index].inUse = false;
                size_--;
                return true;
            }
            // key not found, try next slot
            index = (index + 1) % capacity_;
            i++;
        }
        return false;

}





template <class TYPE>


bool LPTable<TYPE>::find(const std::string& key, TYPE& value){


int index = hash(key);
        int i = 0;
        while (i < capacity_) {
            if (records_[index].inUse && records_[index].key == key) {
                // key found, set value and return true
                value = records_[index].value;
                return true;
            }
            // key not found, try next slot
            index = (index + 1) % capacity_;
            i++;
        }
        return false;

}





template <class TYPE>


const LPTable<TYPE>& LPTable<TYPE>::operator=(const LPTable<TYPE>& other){


if (this != &other) {
            capacity_ = other.capacity_;
            size_ = other.size_;
            delete[] records_;
            records_ = new Record[capacity_];
            for (int i = 0; i < capacity_; i++) {
                records_[i] = other.records_[i];
            }
        }
        return *this;




}





template <class TYPE>


const LPTable<TYPE>& LPTable<TYPE>::operator=(LPTable<TYPE>&& other) {
    if (this != &other) {
        capacity_ = other.capacity_;
        size_ = other.size_;
        delete[] records_;
        records_ = other.records_;
        other.records_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
    }
    return *this;




}





template <class TYPE>


LPTable<TYPE>::~LPTable(){



    delete[] records_;

}





template <class TYPE>


bool LPTable<TYPE>::isEmpty() const {


 return size_ == 0;

}





template <class TYPE>


int LPTable<TYPE>::numRecords() const {

 return size_;


}


template <class TYPE>


int LPTable<TYPE>::capacity() const {


 return capacity_;


}


template <class TYPE>


double LPTable<TYPE>::loadFactor() const {


return 0;


}

