#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;

class CDate {
public:
    CDate(int y,
          int m,
          int d)
            : m_Y(y),
              m_M(m),
              m_D(d) {
    }

    int CompareTo(const CDate &other) const {
        if (m_Y != other.m_Y)
            return (other.m_Y < m_Y) - (m_Y < other.m_Y);
        if (m_M != other.m_M)
            return (other.m_M < m_M) - (m_M < other.m_M);
        return (other.m_D < m_D) - (m_D < other.m_D);
    }

    friend ostream &operator<<(ostream &os,
                               const CDate &d) {
        return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
    }

private:
    int m_Y;
    int m_M;
    int m_D;
};


enum class ESortKey {
    NAME,
    BIRTH_DATE,
    ENROLL_YEAR
};
#endif /* __PROGTEST__ */

//---------------------------------------------------------mine---------------------------------------------------------

class CStudent {
public:
    CStudent(const string &name,
             const CDate &born,
             int enrolled);

    bool operator==(const CStudent &other) const;

    bool operator!=(const CStudent &other) const;

    bool operator < (const CStudent & other)const;

    void setOrder(uint64_t p_Order ){ added = p_Order; }

    const string &getFullName() const { return g_FullName; }

    const CDate &getBirthDate() const { return g_BirthDate; }

    const int getEnrolled() const { return g_Enrolled; }

    const uint64_t getOrder()const{ return added; }

private:
    string g_FullName;
    CDate g_BirthDate;
    int g_Enrolled;
    mutable uint64_t added;
};

CStudent::CStudent(const string &name, const CDate &born, int enrolled) : g_FullName(name), g_BirthDate(born),
                                                                          g_Enrolled(enrolled) {
}

bool CStudent::operator==(const CStudent &other) const {
    return !(g_FullName.compare(other.getFullName()) || g_BirthDate.CompareTo(other.getBirthDate()) ||
             g_Enrolled != other.getEnrolled());
}

bool CStudent::operator!=(const CStudent &other) const {
    return g_FullName.compare(other.getFullName()) || g_BirthDate.CompareTo(other.getBirthDate()) ||
    g_Enrolled != other.getEnrolled();
}

bool CStudent::operator<(const CStudent &other)const {
    if(g_FullName.compare(other.getFullName()))
        return g_FullName < other.getFullName();
    else if(g_BirthDate.CompareTo(other.getBirthDate()))
        return g_BirthDate.CompareTo(other.getBirthDate());
    else if(g_Enrolled!=other.getEnrolled())
        return  g_Enrolled<other.getEnrolled();

    else return true;
}

class CFilter {
public:
    CFilter(void){}

    CFilter &Name(const string &name){}

    CFilter &BornBefore(const CDate &date){}

    CFilter &BornAfter(const CDate &date){}

    CFilter &EnrolledBefore(int year){}

    CFilter &EnrolledAfter(int year){}

private:
    // todo
};

class CSort {
public:

    CSort(void):empty(true){}

     CSort& AddKey (ESortKey key, bool ascending = true) {

         empty=false;

         switch  (key){
             case ESortKey ::NAME:
                 keyList.push_back({0, ascending});
                 break;
             case ESortKey ::BIRTH_DATE:
                 keyList.push_back({1,ascending});
                 break;
             case ESortKey ::ENROLL_YEAR:
                 keyList.push_back({2,ascending});
                 break;
         }

          return *this;
     }

    bool empty;
    vector<pair<int, bool>> keyList;
};


class StudentComparator{
public:

    StudentComparator():g_Options(){}

    StudentComparator(const CSort & p_Sort):g_Options(p_Sort){}

    bool operator() (const CStudent & s1, const CStudent & s2)const{
            for (auto & l_SortOption : g_Options.keyList) {
                switch (l_SortOption.first) {
                    case 0:
                        if (s1.getFullName().compare(s2.getFullName())) {
                            if (l_SortOption.second ? s1.getFullName() < s2.getFullName()
                                                    : s1.getFullName() > s2.getFullName())
                                return true;
                        } else
                            continue;
                        break;
                    case 1:
                        if (s1.getBirthDate().CompareTo(s2.getBirthDate())) {
                            if (l_SortOption.second ? s1.getBirthDate().CompareTo(s2.getBirthDate()) < 0
                                                    : s2.getBirthDate().CompareTo(s1.getBirthDate()) < 0)
                                return true;
                        } else
                            continue;
                        break;
                    case 2:
                        if(s1.getEnrolled()!=s2.getEnrolled()) {
                            if (l_SortOption.second ? s1.getEnrolled() < s2.getEnrolled()
                                                    : s2.getEnrolled() < s1.getEnrolled())
                                return true;
                        }else
                            continue;
                        break;
                    default:
                        break;
                }

                return false;
            }

        return s1.getOrder()<s2.getOrder();

    }

    CSort g_Options;
};

class CStudyDept {
public:
    CStudyDept(void){
        g_StudentCnt=0;
    }

    bool AddStudent(const CStudent &x){

        //CStudent * st  = new CStudent(x);

        //names[ x.getFullName() ].insert(st);

        CStudent s = x;
        s.setOrder(++g_StudentCnt);

        if(g_Students.count(x))
            return false;

        g_Students.insert(s);
        return true;
    }

    bool DelStudent(const CStudent &x){

        if(!g_Students.count(x))
            return false;

        g_Students.erase(x);
        return true;
    }

    list<CStudent> Search(const CFilter &flt,
                          const CSort &sortOpt) const{

        list<CStudent> l_List;


            for (const CStudent & s : g_Students)
                l_List.push_back(s);


        l_List.sort(StudentComparator(sortOpt));

        return l_List;
    }

    set<string> Suggest(const string &name) const{}

private:
    uint64_t g_StudentCnt;
    set<CStudent,StudentComparator> g_Students;
    map<string,set<CStudent*>> names;
    map<string,set<CStudent*>> dates;
    map<uint64_t,set<CStudent*>> enr;
};


//------------------------------------------------------end mine--------------------------------------------------------

#ifndef __PROGTEST__

int main(void) {
    CStudyDept x0;
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2016)));

    //revision 1

    assert (x0.AddStudent(CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014)));
    assert (x0.AddStudent(CStudent("John Taylor", CDate(1981, 6, 30), 2012)));
    assert (x0.AddStudent(CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)));
    assert (x0.AddStudent(CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017)));
    assert (x0.AddStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.AddStudent(CStudent("James Bond", CDate(1982, 7, 16), 2013)));
    assert (x0.AddStudent(CStudent("James Bond", CDate(1981, 8, 16), 2013)));
    assert (x0.AddStudent(CStudent("James Bond", CDate(1981, 7, 17), 2013)));
    assert (x0.AddStudent(CStudent("James Bond", CDate(1981, 7, 16), 2012)));
    assert (x0.AddStudent(CStudent("Bond James", CDate(1981, 7, 16), 2013)));
    assert (x0.Search(CFilter(), CSort()) == (list<CStudent>
            {
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    assert (x0.Search(CFilter(), CSort().AddKey(ESortKey::NAME, true)) == (list<CStudent>
            {
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    assert (x0.Search(CFilter(), CSort().AddKey(ESortKey::NAME, false)) == (list<CStudent>
            {
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    assert (x0.Search(CFilter(),
                      CSort().AddKey(ESortKey::ENROLL_YEAR, false).AddKey(ESortKey::BIRTH_DATE, false).AddKey(
                              ESortKey::NAME, true)) == (list<CStudent>
            {
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));

    //revision 2

    assert (x0.Search(CFilter().Name("james bond"),
                      CSort().AddKey(ESortKey::ENROLL_YEAR, false).AddKey(ESortKey::BIRTH_DATE, false).AddKey(
                              ESortKey::NAME, true)) == (list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012)
            }));
    assert (x0.Search(CFilter().BornAfter(CDate(1980, 4, 11)).BornBefore(CDate(1983, 7, 13)).Name("John Taylor").Name(
            "james BOND"), CSort().AddKey(ESortKey::ENROLL_YEAR, false).AddKey(ESortKey::BIRTH_DATE, false).AddKey(
            ESortKey::NAME, true)) == (list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
            }));
    assert (x0.Search(CFilter().Name("james"), CSort().AddKey(ESortKey::NAME, true)) == (list<CStudent>
            {
            }));
    assert (x0.Suggest("peter") == (set<string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor",
                    "Peter Taylor"
            }));
    assert (x0.Suggest("bond") == (set<string>
            {
                    "Bond James",
                    "James Bond"
            }));
    assert (x0.Suggest("peter joHn") == (set<string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }));
    assert (x0.Suggest("peter joHn bond") == (set<string>
            {
            }));
    assert (x0.Suggest("pete") == (set<string>
            {
            }));
    assert (x0.Suggest("peter joHn PETER") == (set<string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }));
    assert (!x0.AddStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.DelStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.Search(CFilter().BornAfter(CDate(1980, 4, 11)).BornBefore(CDate(1983, 7, 13)).Name("John Taylor").Name(
            "james BOND"), CSort().AddKey(ESortKey::ENROLL_YEAR, false).AddKey(ESortKey::BIRTH_DATE, false).AddKey(
            ESortKey::NAME, true)) == (list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
            }));
    assert (!x0.DelStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));

    return 0;
}

#endif /* __PROGTEST__ */