#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
class InvalidDateException
{
};
#endif /* __PROGTEST__ */

// uncomment if your code implements the overloaded suffix operators
 #define TEST_LITERALS

//*****************************************************mine*************************************************************

const uint8_t DAYSINMONTH[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
const uint16_t DAYSINMONTHSUM[]={0,0,31,59,90,120,151,181,212,243,273,304,334};

class Date{

public:
    int g_Year;
    int g_Month;
    int g_Day;

    Date(int y,int m,int d):g_Year(y),g_Month(m),g_Day(d){};

};

class UDate{

public:

    vector<Date*> g_UDate;

    UDate(int y,int m, int d){
        g_UDate.push_back(new Date(y,m,d));
    };

    ~UDate(){
        for(Date * item : g_UDate)
            delete item;
    }

    UDate &operator + (const UDate & p_UDate){
        this->g_UDate.push_back(new Date(p_UDate.g_UDate[0]->g_Year,p_UDate.g_UDate[0]->g_Month,p_UDate.g_UDate[0]->g_Day));

        return *this;
    }

    UDate &operator - (const UDate & p_UDate){
        return *this + ( - UDate(p_UDate.g_UDate[0]->g_Year,p_UDate.g_UDate[0]->g_Month,p_UDate.g_UDate[0]->g_Day));
    }

    UDate &operator-(){
        g_UDate[0]->g_Year*=-1;
        g_UDate[0]->g_Month*=-1;
        g_UDate[0]->g_Day*=-1;

        return *this;
    }

};

class CDate
{
public:
    CDate(uint64_t p_Year,uint64_t p_Month,uint64_t p_Day);

    //Operators declaration

    // operator(s) +

    CDate operator + (const Date * p_Date);

    CDate operator + ( const UDate & p_UDate);

    // operator(s) -

    CDate  operator - (const UDate & p_UDate);

    long int operator - (const CDate & p_other);

    // operator ==

    bool operator == (const CDate & p_other);

    // operator !=

    bool operator != (const CDate & p_other);

    // operator <

    bool operator < (const CDate & p_other);

    // operator(s) +=

    CDate & operator += (const UDate & p_other);

    //operator <<



    //Functions declaration

    void setLeapYear(const uint64_t p_Year);

    bool isLeapYear();

    const uint64_t getG_Year() const;

    void setG_Year(const uint64_t & p_Year);

    const uint8_t getG_Month() const;

    void setG_Month(const uint8_t & p_Month);

    const uint8_t getG_Day() const;

    void setG_Day(const uint8_t & p_Day);

    long int DateToDays()const;

    long int rdn()const;

    CDate dtf(long d) {
        long y, ddd, mi;

        y = (10000*d + 14780)/3652425;
        ddd = d - (y*365 + y/4 - y/100 + y/400 - y/4000);
        if (ddd < 0) {
            y--;
            ddd = d - (y*365 + y/4 - y/100 + y/400);
        }
        mi = (52 + 100*ddd)/3060;

        return CDate(y + (mi + 2)/12,(mi + 2)%12 + 1,ddd - (mi*306 + 5)/10 + 1);
    }

    long gday(int p_y,int p_m,int p_d) {
        long  l_y = p_y, l_m = p_m;

        l_m = (l_m + 9)%12;
        l_y = l_y - l_m/10;
        return l_y*365 + l_y/4 - l_y/100 + l_y/400 - l_y/4000 + (l_m*306 + 5)/10 + (p_d - 1);
    }


private:
    uint64_t g_Year;
    uint64_t g_Month;
    uint64_t g_Day;

    bool g_LeapYear;
};

//Operators realization-----------------------------------------mine-----------------------------------------------------

// operators +

CDate CDate::operator+(const UDate &p_UDate) {
    for(Date * l_Item : p_UDate.g_UDate)
        *this = *this + l_Item;

    return *this;
}

CDate CDate::operator+(const Date * p_Date) {

    CDate l_(getG_Year(),getG_Month(),getG_Day());
    int l_nMonth = g_Month;

    if(p_Date->g_Year && !p_Date->g_Month){
        l_.setLeapYear(g_Year+p_Date->g_Year);
        l_.setG_Year(g_Year+p_Date->g_Year);
        l_.setG_Day(g_Day);
    }
    else if((p_Date->g_Month>0 || p_Date->g_Month<0) && p_Date->g_Month+(int)g_Month>0) {
        if (!((g_Month + p_Date->g_Month) % 12)) {
            l_.setG_Month(12);
            l_nMonth -= 12;
        } else l_.setG_Month((g_Month + p_Date->g_Month) % 12);
        l_.setG_Year((g_Year + p_Date->g_Year + (l_nMonth + p_Date->g_Month) / 12));
        l_.setLeapYear(g_Year);
        l_.setG_Day(g_Day);

    }else if(p_Date->g_Month<0 && p_Date->g_Month+(int)g_Month<0){
        l_nMonth = p_Date->g_Month * (-1);
         if(!((l_nMonth-g_Month)%12)){
             l_.setG_Month(12);
             l_nMonth+=12;
         }else l_.setG_Month(12-(l_nMonth-g_Month)%12);
        l_.setG_Year(g_Year-(l_nMonth-g_Month)/12-1);
        l_.setLeapYear(l_.getG_Year());
        l_.setG_Day(g_Day);
    }else if(p_Date->g_Day && !p_Date->g_Month){

        l_ = dtf(gday(getG_Year(),getG_Month(),getG_Day())+p_Date->g_Day);

    }

    return l_;

}

//operators -

CDate CDate::operator-(const UDate &p_UDate) {

    return CDate(getG_Year(),getG_Month(),getG_Day())+( - UDate(p_UDate.g_UDate[0]->g_Year,p_UDate.g_UDate[0]->g_Month,p_UDate.g_UDate[0]->g_Day));
}

long int CDate::operator-(const CDate &p_other) {
    if(*this<p_other)
        return (p_other.rdn() - this->rdn()) * -1;
    else return this->rdn() - p_other.rdn();
}

//operators +=

CDate& CDate::operator+=(const UDate &p_other) {
    for (Date * l_Item : p_other.g_UDate)
        *this = *this + l_Item;

    return *this;
}

//operator ==

bool CDate::operator==(const CDate &p_other) {
    if(g_Year!=p_other.getG_Year() || g_Month!=p_other.getG_Month() || g_Day!=p_other.getG_Day())
        return false;
    return true;
}

//operator !=

bool CDate::operator!=(const CDate &p_other) {
    if(g_Year==p_other.getG_Year() && g_Month==p_other.getG_Month() && g_Day==p_other.getG_Day())
        return false;
    else return true;
}

//ooperator <

bool CDate::operator<(const CDate &p_other) {
    if(g_Day<p_other.getG_Day())
        return true;
    else if(g_Month<p_other.getG_Month())
        return true;
    else if(g_Day<p_other.getG_Day())
        return true;
    else return false;
}

//-------------------------------------------------------------output stream--------------------------------------------

ostringstream& operator << (ostringstream &p_ss,const CDate & p_Date) {

    p_ss << p_Date.getG_Year() << "-";
    if(p_Date.getG_Month()<10)
        p_ss << 0 << (int)(p_Date.getG_Month()) << "-";
    else p_ss << (int)(p_Date.getG_Month()) << "-";
    if(p_Date.getG_Day()<10)
        p_ss << 0 << (int)p_Date.getG_Day();
    else p_ss << (int)p_Date.getG_Day();

    return p_ss;
}

ostream& operator<<(basic_ostream<char,std::char_traits<char >> & p_ss,const CDate p_Date){

    p_ss << to_string(p_Date.getG_Year()) << "-";
    if(p_Date.getG_Month()<10)
        p_ss << 0 << (int)(p_Date.getG_Month()) << "-";
    else p_ss << (int)(p_Date.getG_Month()) << "-";
    if(p_Date.getG_Day()<10)
        p_ss << 0 << (int)p_Date.getG_Day();
    else p_ss << (int)p_Date.getG_Day();

    return p_ss;
}

//---------------------------------------------------------LITERALS-----------------------------------------------------

UDate operator "" _year(unsigned long long p_Year){
    return UDate(p_Year,0,0);
}

UDate operator "" _years(unsigned long long p_Year){
    return UDate(p_Year,0,0);
}

UDate operator "" _month(unsigned long long p_Month){
    return UDate(0,p_Month,0);
}

UDate operator "" _months(unsigned long long p_Month){
    return UDate(0,p_Month,0);
}

UDate operator "" _day(unsigned long long p_Day){
    return UDate(0,0,p_Day);
}

UDate operator "" _days(unsigned long long p_Day){
    return UDate(0,0,p_Day);
}


//Functions realization-------------------------------------------------------------------------------------------------

CDate::CDate(uint64_t p_Year, uint64_t p_Month, uint64_t p_Day):g_Year(1600),g_Month(1),g_Day(1) {

    setLeapYear(p_Year);

    setG_Year(p_Year);
    setG_Month(p_Month);
    setG_Day(p_Day);
}


void CDate::setG_Year(const uint64_t &p_Year) {
    if(p_Year<1600)
        throw InvalidDateException();
    g_Year=p_Year;
}

const uint64_t CDate::getG_Year() const {
    return g_Year;
}

void CDate::setG_Month(const uint8_t &p_Month) {
    if(p_Month<1 || p_Month>12)
        throw InvalidDateException();
    g_Month=p_Month;
}

const uint8_t CDate::getG_Month() const {
    return g_Month;
}

void CDate::setG_Day(const uint8_t &p_Day) {
    if(g_Month==2 && ( p_Day<1 || p_Day>DAYSINMONTH[g_Month]+g_LeapYear))
        throw InvalidDateException();
    else if(g_Month!=2 && ( p_Day<1 || p_Day>DAYSINMONTH[g_Month]))
        throw InvalidDateException();
    g_Day=p_Day;
}

const uint8_t CDate::getG_Day() const {
    return g_Day;
}

void CDate::setLeapYear(uint64_t p_Year) {

    g_LeapYear=false;

    if(!(p_Year%4))
        g_LeapYear=true;
    if(!(p_Year%100))
        g_LeapYear=false;
    if(!(p_Year%400))
        g_LeapYear=true;
    if(!(p_Year%4000))
        g_LeapYear=false;

}

bool CDate::isLeapYear() {
    return g_LeapYear;
}

/*long int CDate::DateToDays()const{

    uint64_t l_dy = getG_Year();

    long int l_days = l_dy*365 + l_dy/4 - l_dy/100 + l_dy/400 - l_dy/4000 + DAYSINMONTHSUM[getG_Month()] + getG_Day();
    if(g_LeapYear && g_Month>2)
        l_days++;

    return l_days;
}*/

long int CDate::rdn()const {

    int y = g_Year,m=g_Month;

    if(m<3){
        y--;
        m+=12;
    }

    long int del = 365*y + y/4 - y/100 + y/400 - y/4000 + (153*m - 457)/5 + g_Day - 306;
    return del;
}



//----------------------------------------------------------------------------------------------------------------------

UDate Year(int p_Year){
    return UDate(p_Year,0,0);
}

UDate Month(int p_Month){
    return UDate(0,p_Month,0);
}

UDate Day(int p_Day){
    return UDate(0,0,p_Day);
}


//-------------------------------------------------mine-----------------------------------------------------------------

#ifndef __PROGTEST__
string             toString                                ( const CDate     & x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

int                main                                    ( void )
{
    CDate tmp ( 2000, 1, 1 );
    string d = toString(CDate(2500,12,21));
    //assert( tmp.DateToDays()==146097);
    //assert ( CDate(2007,6,13).DateToDays()==148817);
    assert ( toString ( CDate ( 2000, 1, 1 ) )  == "2000-01-01" );
    assert ( toString ( CDate ( 2500, 12, 21 ) )  == "2500-12-21" );
    assert ( toString ( CDate ( 1685, 7, 11 ) )  == "1685-07-11" );
    try
    {
        tmp = CDate ( 1900, 2, 29 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    assert ( toString ( CDate ( 2000, 2, 29 ) )  == "2000-02-29" );
    assert ( toString ( CDate ( 2004, 2, 29 ) )  == "2004-02-29" );
    try
    {
        tmp = CDate ( 4000, 2, 29 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    //UDate tm = daytodate(1234567);
    assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 1 ) )  == "2019-03-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + 1_year )  == "2019-03-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 7 ) )  == "2025-03-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 73 ) )  == "2091-03-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 3 ) )  == "2018-06-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 1 ) )  == "2018-04-15" );
    assert ( toString ( CDate ( 2018, 11, 15 ) + Month ( 4 ) )  == "2019-03-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 285 ) )  == "2041-12-15" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 1 ) )  == "2018-03-16" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 12 ) )  == "2018-03-27" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 1234567 ) )  == "5398-05-02" );
    assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 3 ) + Year ( 2 ) + Month ( 3 ) + Day ( 5 ) + Month ( 11 ) )  == "2021-05-23" );
    try
    {
        tmp = CDate ( 2000, 2, 29 ) + Year ( 300 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    try
    {
        tmp = CDate ( 2000, 3, 30 ) + Month ( 11 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    assert ( toString ( CDate ( 2001, 2, 20 ) + Day ( 9 ) )  == "2001-03-01" );
    assert ( toString ( CDate ( 2000, 1, 1 ) + Day ( 28 ) + Month ( 1 ) )  == "2000-02-29" );
    assert ( toString ( CDate ( 1999, 1, 1 ) + Year ( 1 ) + Day ( 28 ) + Month ( 1 ) )  == "2000-02-29" );
    assert ( toString ( CDate ( 2001, 1, 1 ) + Day ( 1095 ) + Day ( 28 ) + Month ( 1 ) )  == "2004-02-29" );
    try
    {
        tmp = CDate ( 2001, 1, 1 ) + Day ( 28 ) + Month ( 1 ) + Day ( 1095 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    try
    {
        tmp = CDate ( 2000, 1, 1 ) + Day ( 28 ) + Month ( 1 ) + Year ( 1 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    tmp = CDate ( 2000, 1, 1 );
    assert ( toString ( tmp - Year ( 5 ) + Month ( 2 ) )  == "1995-03-01" );
    assert ( toString ( tmp ) == "2000-01-01" );
    assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 1 ) - Month ( 3 ) - Day ( 10 ) )  == "1998-09-21" );
    assert ( toString ( CDate ( 2000, 1, 1 )  - Year ( 2 ) - Month ( -3 ) + Day ( -10 ))  == "1998-03-22" );
    assert ( toString ( CDate ( 2000, 1, 1 ) + Day ( 59 ) - Month ( 1 ) - Year ( 2 ) )  == "1998-01-29" );
    try
    {
        tmp = CDate ( 2000, 1, 1 ) + Day ( 59 ) - Year ( 2 ) - Month ( 1 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    tmp = CDate ( 2000, 1, 1 );
    tmp +=  - Year ( 2 ) - Month ( -3 ) + Day ( -10 );
    assert ( toString ( tmp ) == "1998-03-22" );
    tmp = CDate ( 2000, 1, 1 );
    tmp += Day ( 59 ) - Month ( 1 ) - Year ( 2 );
    assert ( toString ( tmp ) == "1998-01-29" );
    try
    {
        tmp = CDate ( 2000, 1, 1 );
        tmp += Day ( 59 ) - Year ( 2 ) - Month ( 1 );
        assert ( "Missing exception" == NULL );
    }
    catch ( const InvalidDateException & e )
    {
    }
    tmp = CDate ( 2018, 3, 15 ) + Day ( -3 );
    assert ( toString ( tmp ) == "2018-03-12" );
    assert ( !( CDate ( 2018, 3, 15 ) == CDate ( 2000, 1, 1 ) ) );
    assert ( CDate ( 2018, 3, 15 ) != CDate ( 2000, 1, 1 ) );
    assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2000, 1, 1 ) ) );
    assert ( CDate ( 2018, 3, 15 ) == CDate ( 2018, 3, 15 ) );
    assert ( !( CDate ( 2018, 3, 15 ) != CDate ( 2018, 3, 15 ) ) );
    assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2018, 3, 15 ) ) );
    assert ( !( CDate ( 2018, 3, 15 ) + Day ( 1 ) == CDate ( 2018, 3, 15 ) ) );
    assert ( CDate ( 2018, 3, 15 ) + Day ( 1 ) != CDate ( 2018, 3, 15 ) );
    assert ( !( CDate ( 2018, 3, 15 ) + Day ( 1 ) < CDate ( 2018, 3, 15 ) ) );
    assert ( !( CDate ( 2018, 3, 15 ) + Day ( -1 ) == CDate ( 2018, 3, 15 ) ) );
    assert ( CDate ( 2018, 3, 15 ) + Day ( -1 ) != CDate ( 2018, 3, 15 ) );
    assert ( CDate ( 2018, 3, 15 ) + Day ( -1 ) < CDate ( 2018, 3, 15 ) );
    assert ( CDate ( 2018, 3, 15 ) - CDate ( 2000, 1, 1 ) == 6648 );
    assert ( CDate ( 2000, 1, 1 ) - CDate ( 2018, 3, 15 ) == -6648 );
    assert ( CDate ( 2018, 3, 15 ) + Year ( 3 ) + Month ( -18 ) - CDate ( 2000, 1, 1 ) == 7197 );
    assert ( CDate ( 5398, 5, 2 ) - CDate ( 2018, 3, 15 ) == 1234567 );

#ifdef TEST_LITERALS
    assert ( toString ( CDate ( 2000, 1, 1 ) )  == "2000-01-01" );
  assert ( toString ( CDate ( 2500, 12, 21 ) )  == "2500-12-21" );
  assert ( toString ( CDate ( 1685, 7, 11 ) )  == "1685-07-11" );
  try
  {
    tmp = CDate ( 1900, 2, 29 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2000, 2, 29 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 2004, 2, 29 ) )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 4000, 2, 29 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_year )  == "2019-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 7_years )  == "2025-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 73_years )  == "2091-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 3_months )  == "2018-06-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_month )  == "2018-04-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 285_months )  == "2041-12-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_day )  == "2018-03-16" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 12_days )  == "2018-03-27" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1234567_days )  == "5398-05-02" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 3_days + 2_years + 3_months + 5_days + 11_months )  == "2021-05-23" );
  try
  {
    tmp = CDate ( 2000, 2, 29 ) + 300_years;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 3, 30 ) + 11_months;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2001, 2, 20 ) + 9_days )  == "2001-03-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + 28_days + 1_month )  == "2000-02-29" );
  assert ( toString ( CDate ( 1999, 1, 1 ) + 1_year + 28_days + 1_month )  == "2000-02-29" );
  assert ( toString ( CDate ( 2001, 1, 1 ) + 1095_days + 28_days + 1_month )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 2001, 1, 1 ) + 28_days + 1_month + 1095_days;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + 28_days + 1_month + 1_year;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  assert ( toString ( tmp - 5_years + 2_months )  == "1995-03-01" );
  assert ( toString ( tmp ) == "2000-01-01" );
    assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 1 ) - Month ( 3 ) - Day ( 10 ) )  == "1998-09-21" );
    assert ( toString ( CDate ( 2000, 1, 1 )  - Year ( 2 ) - Month ( -3 ) + Day ( -10 ))  == "1998-03-22" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - 1_year - 3_months - 10_days )  == "1998-09-21" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - 2_years - ( -3_months ) + ( -10_days ) )  == "1998-03-22" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + 59_days - 1_month - 2_years )  == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + 59_days - 2_years - 1_month;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  tmp +=  - 2_years - ( -3_months ) + ( -10_days );
  assert ( toString ( tmp ) == "1998-03-22" );
  tmp = CDate ( 2000, 1, 1 );
  tmp += 59_days - 1_month - 2_years;
  assert ( toString ( tmp ) == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 );
    tmp += 59_days - 2_years - 1_month;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2018, 3, 15 ) + ( -3_days );
  assert ( toString ( tmp ) == "2018-03-12" );
  assert ( !( CDate ( 2018, 3, 15 ) == CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) != CDate ( 2000, 1, 1 ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) == CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) != CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + 1_day == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + 1_day != CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) + 1_day < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + ( -1_day ) == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + ( -1_day ) != CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) + ( -1_day ) < CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) - CDate ( 2000, 1, 1 ) == 6648 );
  assert ( CDate ( 2000, 1, 1 ) - CDate ( 2018, 3, 15 ) == -6648 );
  assert ( CDate ( 2018, 3, 15 ) + 3_years + ( -18_months ) - CDate ( 2000, 1, 1 ) == 7197 );
  assert ( CDate ( 5398, 5, 2 ) - CDate ( 2018, 3, 15 ) == 1234567 );
  ostringstream oss;
  oss << setfill ( 'x' ) << left << hex << CDate ( 2000, 1, 1 ) << ' ' << setw ( 10 ) << 65536;
  assert ( oss . str () == "2000-01-01 10000xxxxx" );
#endif /* TEST_LITERALS */
    return 0;
}
#endif /* __PROGTEST__ */