#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class Person{

private:
    string g_Name;
    string g_Surname;
    string g_Email;
    unsigned int  g_Salary;
public:
    Person(const string    & name,
           const string    & surname,
           const string    & email,
           unsigned int      salary){
        setG_Name(name);
        setG_Surname(surname);
        setG_Email(email);
        setG_Salary(salary);
    }

    const string getFullName()const{
        return g_Name+g_Surname;
    }

    const string &getG_Name() const {
        return g_Name;
    }

    void setG_Name(const string &g_Name){
        Person::g_Name = g_Name;
    }

    const string &getG_Surname() const {
        return g_Surname;
    }

    void setG_Surname(const string &g_Surname) {
        Person::g_Surname = g_Surname;
    }

    const string &getG_Email() const {
        return g_Email;
    }

    void setG_Email(const string &g_Email) {
        Person::g_Email = g_Email;
    }

    unsigned int getG_Salary() const {
        return g_Salary;
    }

    void setG_Salary(unsigned int g_Salary) {
        Person::g_Salary = g_Salary;
    }
};



class CPersonalAgenda
{
public:
    CPersonalAgenda  ( void );
    ~CPersonalAgenda ( void );
    bool          Add              ( const string    & name,
                                     const string    & surname,
                                     const string    & email,
                                     unsigned int      salary );
    bool          Del              ( const string    & name,
                                     const string    & surname );
    bool          Del              ( const string    & email );
    bool          ChangeName       ( const string    & email,
                                     const string    & newName,
                                     const string    & newSurname );
    bool          ChangeEmail      ( const string    & name,
                                     const string    & surname,
                                     const string    & newEmail );
    bool          SetSalary        ( const string    & name,
                                     const string    & surname,
                                     unsigned int      salary );
    bool          SetSalary        ( const string    & email,
                                     unsigned int      salary );
    unsigned int  GetSalary        ( const string    & name,
                                     const string    & surname ) const;
    unsigned int  GetSalary        ( const string    & email ) const;
    bool          GetRank          ( const string    & name,
                                     const string    & surname,
                                     int             & rankMin,
                                     int             & rankMax ) const;
    bool          GetRank          ( const string    & email,
                                     int             & rankMin,
                                     int             & rankMax ) const;
    bool          GetFirst         ( string          & outName,
                                     string          & outSurname ) const;
    bool          GetNext          ( const string    & name,
                                     const string    & surname,
                                     string          & outName,
                                     string          & outSurname ) const;


    //Other helpfull functions

    bool pushElementByAlphabet( vector <Person*> & p_List,
                                Person * p_Person,
                                uint8_t p_Variant);

    bool compareByFullName  (   const Person * p_P1,const Person * p_P2)const;

    bool compareByEmail(        const Person * p_P1,const Person * p_P2)const;

    bool equal(                 const Person * p_P1,const Person * p_P2,bool p_Variant)const;

    bool implementedBinarySearch(const vector<Person *> & p_List,
                                 Person * &p_Person,
                                 uint64_t & p_Position,
                                 bool p_Variant)const;

    bool isDublicate(            const vector<Person*> p_List,
                                 Person * p_Person,
                                 const bool p_Variant)const;


private:
    vector <Person*> g_OrderedByFullName;
    vector <Person*> g_OrderedByEmail;
};


CPersonalAgenda::CPersonalAgenda() {}

CPersonalAgenda::~CPersonalAgenda() {
    for (Person * l_Item : g_OrderedByFullName)
        delete l_Item;
}

bool CPersonalAgenda::Add(const string &name, const string &surname, const string &email, unsigned int salary) {

    if(!salary)
        return false;

    Person * l_Person = new Person(name,surname,email,salary);


    if(isDublicate(g_OrderedByFullName,l_Person,1) || isDublicate(g_OrderedByEmail,l_Person,0))
    {
        delete l_Person;
        return false;
    }

    if(g_OrderedByFullName.size()==0){
        g_OrderedByFullName.push_back(l_Person);
        g_OrderedByEmail.push_back(l_Person);
        return true;
    }
    else if(pushElementByAlphabet(g_OrderedByFullName,l_Person,1) && pushElementByAlphabet(g_OrderedByEmail,l_Person,0))
        return true;
    else{
        delete l_Person;
        return false;
    }

}

bool CPersonalAgenda::Del(const string &name, const string &surname) {

    uint64_t  l_Index=0;
    Person * l_Tmp=NULL,*p = new Person(name,surname,"",0);

    if(!implementedBinarySearch(g_OrderedByFullName,p,l_Index,1))
    {
        delete p;
        return false;}

    l_Tmp = g_OrderedByFullName[l_Index];
    while(l_Index<g_OrderedByFullName.size()-1){
        g_OrderedByFullName[l_Index] = g_OrderedByFullName[l_Index+1];
        l_Index++;
    }

    implementedBinarySearch(g_OrderedByEmail,l_Tmp,l_Index,0);

    while(l_Index<g_OrderedByEmail.size()-1){
        g_OrderedByEmail[l_Index] = g_OrderedByEmail[l_Index+1];
        l_Index++;
    }


    delete p;
    delete l_Tmp;
    g_OrderedByFullName.pop_back();
    g_OrderedByEmail.pop_back();
    return true;
}

bool CPersonalAgenda::Del(const string &email) {

    uint64_t  l_Index=0;
    Person * l_Tmp=NULL,*p = new Person("","",email,0);



    if(!implementedBinarySearch(g_OrderedByEmail,p,l_Index,0)){
        delete p;
        return false;
    }

    l_Tmp = g_OrderedByEmail[l_Index];
    while(l_Index<g_OrderedByEmail.size()-1){
        g_OrderedByEmail[l_Index] = g_OrderedByEmail[l_Index+1];
        l_Index++;
    }


    implementedBinarySearch(g_OrderedByFullName,l_Tmp,l_Index,1);

    while(l_Index<g_OrderedByFullName.size()-1){
        g_OrderedByFullName[l_Index] = g_OrderedByFullName[l_Index+1];
        l_Index++;
    }

    delete p;
    delete l_Tmp;
    g_OrderedByEmail.pop_back();
    g_OrderedByFullName.pop_back();
    return true;
}

bool CPersonalAgenda::ChangeName(const string &email, const string &newName, const string &newSurname) {

    uint64_t  l_Index=0;

    Person * p = new Person(newName,newSurname,email,0);

    if(isDublicate(g_OrderedByFullName,p,1) || !implementedBinarySearch(g_OrderedByEmail,p,l_Index,0))
    {
        delete p;
        return false;
    }

    g_OrderedByEmail[l_Index]->setG_Name(newName);
    g_OrderedByEmail[l_Index]->setG_Surname(newSurname);

    sort(g_OrderedByFullName.begin(),g_OrderedByFullName.end(),[](const Person * p_1 , const Person * p_2){
        if(p_1->getG_Surname()==p_2->getG_Surname())
            return p_1->getG_Name()<p_2->getG_Name();
        else
            return p_1->getG_Surname()<p_2->getG_Surname();
    });

    delete p;
    return true;

}

bool CPersonalAgenda::ChangeEmail(const string &name, const string &surname, const string &newEmail) {

    uint64_t  l_Index=0;

    Person * p = new Person(name,surname,newEmail,0);

    if(isDublicate(g_OrderedByEmail,p,0) || !implementedBinarySearch(g_OrderedByFullName,p,l_Index,1))
    {
        delete p;
        return false;
    }

    g_OrderedByFullName[l_Index]->setG_Email(newEmail);

    sort(g_OrderedByEmail.begin(),g_OrderedByEmail.end(),[](const Person * p_1 , const Person * p_2){
        return p_1->getG_Email()<p_2->getG_Email();
    });

    delete p;

    return true;

}

bool CPersonalAgenda::SetSalary(const string &name, const string &surname, unsigned int salary) {

    uint64_t  l_Index=0;

    Person * p = new Person(name,surname,"",0);


    if(!implementedBinarySearch(g_OrderedByFullName,p,l_Index,1))
    {
        delete p;
        return false;
    }

    g_OrderedByFullName[l_Index]->setG_Salary(salary);

    delete p;
    return true;

}

bool CPersonalAgenda::SetSalary(const string &email, unsigned int salary) {

    uint64_t  l_Index=0;

    Person * p  = new Person("","",email,0);

    if(!implementedBinarySearch(g_OrderedByEmail,p,l_Index,0))
    {
        delete p;
        return false;
    }

    g_OrderedByEmail[l_Index]->setG_Salary(salary);

    delete p;
    return true;

}


unsigned int CPersonalAgenda::GetSalary(const string &name, const string &surname) const {

    uint64_t l_Index=0;

    Person * p = new Person(name,surname,"",0);

    if(implementedBinarySearch(g_OrderedByFullName,p,l_Index,1)){
        delete p;
        return g_OrderedByFullName[l_Index]->getG_Salary();
    }
    else {
        delete p;
        return 0;
    }
}

unsigned int CPersonalAgenda::GetSalary(const string &email) const {

    uint64_t l_Index=0;

    Person * p = new Person("","",email,0);

    if(implementedBinarySearch(g_OrderedByEmail,p,l_Index,0)){
        delete p;
        return g_OrderedByEmail[l_Index]->getG_Salary();
    }
    else {
        delete p;
        return 0;
    }

}

bool CPersonalAgenda::GetRank(const string &email, int &rankMin, int &rankMax) const {

    uint64_t l_Index=0,l_salary=0;

    Person * p = new Person("","",email,0);

    if(!implementedBinarySearch(g_OrderedByEmail,p,l_Index, false))
    {
        delete p;
        return false;
    }

    rankMax=rankMin=0;
    l_salary = g_OrderedByEmail[l_Index]->getG_Salary();

    for (Person* l_Item : g_OrderedByEmail)
        if(l_salary==l_Item->getG_Salary()) rankMax++ ;
        else if(l_salary>l_Item->getG_Salary())rankMin++;

    rankMax+=rankMin;
        rankMax--;

    delete p;

    return true;
}

bool CPersonalAgenda::GetRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {
    uint64_t l_Index=0,l_salary=0;

    Person * p = new Person(name,surname,"",1);

    if(!implementedBinarySearch(g_OrderedByFullName,p,l_Index, true))
    {
        delete p;
        return false;
    }

    rankMax=rankMin=0;
    l_salary = g_OrderedByFullName[l_Index]->getG_Salary();

    for (Person* l_Item : g_OrderedByFullName)
        if(l_salary==l_Item->getG_Salary()) rankMax++ ;
        else if(l_salary>l_Item->getG_Salary())rankMin++;

    rankMax+=rankMin;
    rankMax--;

    delete p;
    return true;

}

bool CPersonalAgenda::GetFirst(string &outName, string &outSurname) const {

    if(g_OrderedByFullName.empty())
        return false;

    Person * l_Person = g_OrderedByFullName.front();

    outName = l_Person->getG_Name();
    outSurname = l_Person->getG_Surname();


    return true;

}

bool CPersonalAgenda::GetNext(const string &name, const string &surname, string &outName, string &outSurname) const {

    uint64_t l_Index=0;

    Person * p = new Person(name,surname,"",0);

    if(!implementedBinarySearch(g_OrderedByFullName,p,l_Index,1) || l_Index==g_OrderedByFullName.size()-1)
    {
        delete p;
        return false;
    }

    outName = g_OrderedByFullName[l_Index+1]->getG_Name();
    outSurname = g_OrderedByFullName[l_Index+1]->getG_Surname();

    delete p;
    return true;

}


// -                             ---- -- - - - -- -

bool CPersonalAgenda::pushElementByAlphabet(vector<Person *> & p_List,Person * p_Person,uint8_t p_Variant) {

    Person * l_Tmp = NULL;
    uint64_t  l_ListSize = p_List.size();

    for (uint64_t i = 0 ; i<l_ListSize;++i){
        if(equal(p_List[i],p_Person,p_Variant))
            return false;
        else if((!p_Variant && compareByEmail(p_List[i],p_Person)) || (p_Variant && compareByFullName(p_List[i],p_Person))) {
            l_Tmp = p_List[i];
            p_List[i] = p_Person;
            p_Person = l_Tmp;
        }
        else continue;
    }

    p_List.push_back(p_Person);
    //if(l_Tmp) delete l_Tmp;
    return true;
}

bool CPersonalAgenda::compareByFullName(const Person * p_P1,const Person * p_P2)const {
    if(!p_P1->getG_Surname().compare(p_P2->getG_Surname()))
        return p_P1->getG_Name().compare(p_P2->getG_Name())>0;
    else return p_P1->getG_Surname().compare(p_P2->getG_Surname())>0;
}

bool CPersonalAgenda::compareByEmail(const Person * p_P1,const Person * p_P2)const {
    return p_P1->getG_Email().compare(p_P2->getG_Email())>0;
}

bool CPersonalAgenda::equal(const Person *p_P1, const Person *p_P2,bool p_Variant)const {
    return p_Variant ?
           p_P1->getG_Name()==p_P2->getG_Name() && p_P1->getG_Surname()==p_P2->getG_Surname() :
           p_P1->getG_Email()==p_P2->getG_Email();
}

bool CPersonalAgenda::implementedBinarySearch(const vector<Person *> &p_List, Person * &p_Person,uint64_t & p_Position,bool p_Variant) const {

    uint64_t l_Begin = 0,l_End = p_List.size()-1;
    if(p_List.empty())
        return false;
    else if ((p_Variant && (compareByFullName(p_List[0],p_Person) || compareByFullName(p_Person,p_List[l_End]))))
        return false;
    else if (!p_Variant && (compareByEmail(p_List[0],p_Person) || compareByEmail(p_Person,p_List[l_End])))
        return false;

    while(l_Begin<=l_End){
        p_Position = (l_Begin+l_End)/2;
        if(p_Variant && p_List[p_Position]->getG_Name()==p_Person->getG_Name() && p_List[p_Position]->getG_Surname()==p_Person->getG_Surname()){
            return true;
        }
        else if(!p_Variant && p_List[p_Position]->getG_Email()==p_Person->getG_Email())
            return true;
        else if((p_Variant && compareByFullName(p_List[p_Position],p_Person)) || (!p_Variant && compareByEmail(p_List[p_Position],p_Person)))
            l_End=p_Position-1;
        else
            l_Begin=p_Position+1;
    }

    //delete p_Person;
    return false;
}

bool CPersonalAgenda::isDublicate(const vector<Person *> p_List,Person *p_Person,const bool p_Variant) const {
    uint64_t i;
    return implementedBinarySearch(p_List,p_Person,i,p_Variant);

}


#ifndef __PROGTEST__
int main ( void )
{
    string outName, outSurname;
    int lo, hi;

    CPersonalAgenda b1;
    assert ( b1 . Add ( "John", "Smith", "john", 30000 ) );
    assert ( b1 . Add ( "John", "Miller", "johnm", 35000 ) );
    assert ( b1 . Add ( "Peter", "Smith", "peter", 23000 ) );
    assert ( b1 . GetFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . GetNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( b1 . GetNext ( "John", "Smith", outName, outSurname )
             && outName == "Peter"
             && outSurname == "Smith" );
    assert ( ! b1 . GetNext ( "Peter", "Smith", outName, outSurname ) );
    assert ( b1 . SetSalary ( "john", 32000 ) );
    assert ( b1 . GetSalary ( "john" ) ==  32000 );
    assert ( b1 . GetSalary ( "John", "Smith" ) ==  32000 );
    assert ( b1 . GetRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . GetRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . GetRank ( "johnm", lo, hi )
             && lo == 2
             && hi == 2 );
    assert ( b1 . SetSalary ( "John", "Smith", 35000 ) );
    assert ( b1 . GetSalary ( "John", "Smith" ) ==  35000 );
    assert ( b1 . GetSalary ( "john" ) ==  35000 );
    assert ( b1 . GetRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . GetRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . GetRank ( "johnm", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . ChangeName ( "peter", "James", "Bond" ) );
    assert ( b1 . GetSalary ( "peter" ) ==  23000 );
    assert ( b1 . GetSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . GetSalary ( "Peter", "Smith" ) ==  0 );
    assert ( b1 . GetFirst ( outName, outSurname )
             && outName == "James"
             && outSurname == "Bond" );
    assert ( b1 . GetNext ( "James", "Bond", outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . GetNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . GetNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . ChangeEmail ( "James", "Bond", "james" ) );
    assert ( b1 . GetSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . GetSalary ( "james" ) ==  23000 );
    assert ( b1 . GetSalary ( "peter" ) ==  0 );
    assert ( b1 . Del ( "james" ) );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 0
             && hi == 1 );
    assert ( b1 . Del ( "John", "Miller" ) );
    assert ( b1 . GetRank ( "john", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . GetFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . GetNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . Del ( "john" ) );
    assert ( ! b1 . GetFirst ( outName, outSurname ) );
    assert ( b1 . Add ( "John", "Smith", "john", 31000 ) );
    assert ( b1 . Add ( "john", "Smith", "joHn", 31000 ) );
    assert ( b1 . Add ( "John", "smith", "jOhn", 31000 ) );

    CPersonalAgenda b2;
    assert ( ! b2 . GetFirst ( outName, outSurname ) );
    assert ( b2 . Add ( "James", "Bond", "james", 70000 ) );
    assert ( b2 . Add ( "James", "Smith", "james2", 30000 ) );
    assert ( b2 . Add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( ! b2 . Add ( "James", "Bond", "james3", 60000 ) );
    assert ( ! b2 . Add ( "Peter", "Bond", "peter", 50000 ) );
    assert ( ! b2 . ChangeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . ChangeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . SetSalary ( "Joe", "Black", 90000 ) );
    assert ( ! b2 . SetSalary ( "joe", 90000 ) );
    assert ( b2 . GetSalary ( "Joe", "Black" ) ==  0 );
    assert ( b2 . GetSalary ( "joe" ) ==  0 );
    assert ( ! b2 . GetRank ( "Joe", "Black", lo, hi ) );
    assert ( ! b2 . GetRank ( "joe", lo, hi ) );
    assert ( ! b2 . ChangeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . ChangeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . Del ( "Joe", "Black" ) );
    assert ( ! b2 . Del ( "joe" ) );
    assert ( ! b2 . ChangeName ( "james2", "James", "Bond" ) );
    assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "james" ) );
    assert ( ! b2 . ChangeName ( "peter", "Peter", "Smith" ) );
    assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "peter" ) );
    assert ( b2 . Del ( "Peter", "Smith" ) );
    assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "peter2" ) );
    assert ( ! b2 . SetSalary ( "Peter", "Smith", 35000 ) );
    assert ( b2 . GetSalary ( "Peter", "Smith" ) ==  0 );
    assert ( ! b2 . GetRank ( "Peter", "Smith", lo, hi ) );
    assert ( ! b2 . ChangeName ( "peter", "Peter", "Falcon" ) );
    assert ( ! b2 . SetSalary ( "peter", 37000 ) );
    assert ( b2 . GetSalary ( "peter" ) ==  0 );
    assert ( ! b2 . GetRank ( "peter", lo, hi ) );
    assert ( ! b2 . Del ( "Peter", "Smith" ) );
    assert ( ! b2 . Del ( "peter" ) );
    assert ( b2 . Add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( b2 . GetSalary ( "peter" ) ==  40000 );

    return 0;
}
#endif /* __PROGTEST__ */
