/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "ulimitedint.h"
#include<iostream>
#include<algorithm>
UnlimitedInt::UnlimitedInt(){
    size = 0;
    capacity =0;
    sign = 1;
}
UnlimitedInt::UnlimitedInt(int i) {
    
   if(i<0){
    sign = -1;
   }
   else{
    sign = 1;
   }
    size = 0;
    i = abs(i);

    //  for i = 0
    if (i == 0) {
        size = 1;
        unlimited_int = new int[1];
        unlimited_int[0] = 0;
        return;
    }

    // Convert the integer to a string
    std::string s = std::to_string(i);
    size = s.size();

    unlimited_int = new int[size];
    for (int j = 0; j < size; j++) {
        unlimited_int[j] = s[size - j - 1] - '0';
    }
}

UnlimitedInt::UnlimitedInt(std::string s) {
    size = 0;
    sign = 1;

    if (s[0] == '-') {
        sign = -1;
        s = s.substr(1);  // Remove the negative sign
    }

    size = s.size();
    unlimited_int = new int[size];

    for (int i = 0; i < size; i++) {
        unlimited_int[i] = s[i] - '0';
    }
}
UnlimitedInt::~UnlimitedInt(){
   delete[]  unlimited_int;

}
int UnlimitedInt::get_size(){
    return this->size;
}
int* UnlimitedInt::get_array(){
    return unlimited_int;
}
int UnlimitedInt::get_sign(){
    return this->sign;
}
string UnlimitedInt::to_string(){
    string str = "";
    int*  a = this->get_array();
    for(int i=0;i<this->get_size();i++){
        int number = a[i];
        str+=std::to_string(number);
    }
    return str;
}


UnlimitedInt* helper_add(UnlimitedInt* i1, UnlimitedInt* i2){
    // Converting into string and adding
    // Changing the final output into an UnlimitedInt
    std::string a = i1->to_string();
    std::string b = i2->to_string();
    int carry = 0;
    std::string res = "";
    int i = a.length()-1;
    int j = b.length()-1;
    while(i >= 0 || j >= 0 || carry > 0){
        int dgt1 = (i >= 0) ? a[i] - '0' : 0;
        int dgt2 = (j >= 0) ? b[j] - '0' : 0;
        int sum = dgt1 + dgt2 + carry;
        carry = sum / 10;
        int dgtsum = sum % 10;
        res += std::to_string(dgtsum);
        i--;
        j--;
    }
    
    // Reversing the result string to get the correct order
    std::reverse(res.begin(), res.end());

    return new UnlimitedInt(res);
}


UnlimitedInt* UnlimitedInt::add(UnlimitedInt* i1, UnlimitedInt* i2){
   UnlimitedInt* a = helper_add(i1,i2);
   return a;
}

int Str_compare(UnlimitedInt* i1, UnlimitedInt* i2){
    int size1 = i1->get_size();
    int size2 = i2->get_size();
    int* arr1 = i1->get_array();
    int* arr2 = i2->get_array();
    if (size1 > size2) {
        return 1;
    } else if (size1 < size2) {
        return -1;
    }

    for (int i = size1 - 1; i >= 0; i--) {
        if (arr1[i] > arr2[i]) {
            return 1;
        } else if (arr1[i] < arr2[i]) {
            return -1;
        }
    }
    return 0;
}

UnlimitedInt* helper_sub(UnlimitedInt* i1, UnlimitedInt* i2,bool negans){
    std::string a = i1->to_string();
    std::string b = i2->to_string();

    //bool negans = false;  // Initialize negans for handling -ve values
    
    if (Str_compare(i1, i2) == -1) {
        std::swap(a, b);
        negans = true;
    }
    
    
    int borrow = 0;
    std::string res = "";
    int i = a.length()-1;
    int j = b.length()-1;

    
    while(i>=0||j>=0||borrow>0){
        int dgt1 = (i>=0) ? a[i] - '0': 0; 
        int dgt2 = (i>=0) ? b[i] - '0': 0;
        //if any borrow substract it from the upper dgt
        dgt1-= borrow;
        borrow =0;
        //taking borrow
        if(dgt1 < dgt2){
            dgt1+=10;
            borrow = 1;
        }
        int diff = dgt1 - dgt2;
        //pushing in res
        res +=std::to_string(diff);
        i--;
        j--;
    }
    
    
   // Remove leading zeros
    while (!res.empty() && res[0] == '0') {
        res.erase(0, 1);
    }
    std::reverse(res.begin(), res.end());
    // Create a new UnlimitedInt with the result
    UnlimitedInt* result = new UnlimitedInt(res);
   
    
    return result;
}
        
    

UnlimitedInt* UnlimitedInt::sub(UnlimitedInt* i1, UnlimitedInt* i2){
    bool ans = false;
    
    UnlimitedInt* a = helper_sub(i1,i2,ans);
    if (ans == true){
        a->sign = -1;
    } 
    return a;
}

UnlimitedInt* UnlimitedInt::mul(UnlimitedInt* i1,UnlimitedInt* i2){
    std::string a = i1->to_string();
    std::string b = i2->to_string();

    int* temp1 = i1->get_array();
    int* temp2 = i2->get_array();

    int l1 = i1->get_size();
    int l2 = i2->get_size();

    std::string res(l1+l2,'0');

    //iterate digits of 1st number
    for(int i=l1-1;i>=0;i--){
        int carry = 0;
        int dgt1 = temp1[i];
        //iterate through digits of 2nd number
        for(int j=l2-1;j>=0;j--){
            int dgt2 = temp2[j];
            int product = (dgt1 * dgt2) + (res[i + j + 1] - '0');
            carry = product / 10;
            res[i+j+1] = (product % 10) + '0';
        }
        res[i] +=carry;
    }
    //find 1st non-zero dgt;
    int start = 0;
    while(start<res.size() && res[start] == '0'){
        start++;
    }
    // return string without leading zeros
    if(start == res.size()){
        res = "0";
    }else{
    res =  res.substr(start);
    }
    UnlimitedInt* output = new UnlimitedInt(res);
    return output;
}   

UnlimitedInt* UnlimitedInt::div(UnlimitedInt* i1,UnlimitedInt* i2){
    std::string a = i1->to_string();
    std::string b = i1->to_string();
    //a is divident ,b is divisor
    //variables of division
    std::string res ="";
    int quotient = 0;
    int remainder = 0;
    bool decimal_part = false;
    double decimal =0;
    //iterate through i1
   for(char dgt : a){

    int current = dgt -'0';
    int divided = remainder * 10 + current;
    if(divided >= 0){
        quotient = divided/(b[0] - '0');
        remainder = divided % (b[0] - '0');
    } else{
        quotient = 0;
        remainder = divided;
    }
    res += std:: to_string(quotient);
    }
     UnlimitedInt* A =new  UnlimitedInt(res);
    if(A->get_sign() == -1 && remainder!= 0){
        res[res.size()-1]+=1;
        A = new UnlimitedInt(res);
    }
    return A;

   }




int main(){
    UnlimitedInt* a = new UnlimitedInt(12);
    cout<<a->to_string()<<endl;
    cout<<a->get_sign()<<endl;
    UnlimitedInt* b= new UnlimitedInt("-3");
    cout<<b->to_string()<<endl;
     cout<<b->get_sign()<<endl;
    UnlimitedInt* c =UnlimitedInt::add(a,b);
    cout<<c->to_string()<<endl;
    cout<<c->get_sign()<<endl;
    // UnlimitedInt* e=UnlimitedInt::sub(a,b);
    // cout<<e->to_string()<<endl;
    // cout<<e->get_sign()<<endl;
    // UnlimitedInt* d = UnlimitedInt::mul(a,b);
    // cout<<d->to_string()<<endl;
    // cout<<d->get_sign()<<endl;
    // UnlimitedInt* f = UnlimitedInt::div(a,b);
    // cout<<e->to_string()<<endl;
    // cout<<d->get_sign()<<endl;

}
        
   

