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

void split(const std::string &s,vector<string> & p_Out) {
    string l_Word;
    for (const char c : s)
        if(c==' ')
        {
            if(l_Word.empty())
                continue;
            p_Out.push_back(l_Word);
            l_Word.clear();
        }else
            l_Word+=c;
    p_Out.push_back(l_Word);
}

void vectorToString(vector<string> & v,string & name){
    for (size_t i = 0; i < v.size(); ++i)
        name+=v[i]+" ";
}

string getLowerAndSorted(const string & name){
    string l_name_cp = name;
    transform(name.begin(),name.end(),l_name_cp.begin(),::tolower);
    vector<string> words;
    split(l_name_cp,words);
    sort(words.begin(),words.end());
    l_name_cp.clear();
    vectorToString(words,l_name_cp);
    return l_name_cp;

}

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

    const string & getKey()const{ return g_key; }

private:
    string g_FullName;
    CDate g_BirthDate;
    int g_Enrolled;
    string g_key;
    mutable uint64_t added;
};

CStudent::CStudent(const string &name, const CDate &born, int enrolled) : g_FullName(name), g_BirthDate(born),
                                                                          g_Enrolled(enrolled) {
    g_key=getLowerAndSorted(name);
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

    else return false;
}

class CFilter {
public:
    CFilter(void):g_Empty(true),dbOn(false),daOn(false),g_BornBefore(nullptr),g_BornAfter(nullptr),g_EnrolledBefore(-1),g_EnrolledAfter(-1){}

    ~CFilter(){
        if(g_BornAfter)
            delete g_BornAfter;
        if(g_BornBefore)
            delete g_BornBefore;
    }

    CFilter &Name(const string &name){
        g_Empty=false;
        g_NameFilter.insert(getLowerAndSorted(name));
        return *this;
    }

    CFilter &BornBefore(const CDate &date){
        g_Empty=false;
        dbOn=true;
        g_BornBefore= new CDate(date);
        return *this;
    }

    CFilter &BornAfter(const CDate &date){
        daOn=true;
        g_Empty=false;
        g_BornAfter= new CDate(date);
        return *this;
    }

    CFilter &EnrolledBefore(int year){
        g_Empty=false;
        g_EnrolledBefore=year;
        return *this;
    }

    CFilter &EnrolledAfter(int year){
        g_Empty=false;
        g_EnrolledAfter=year;
        return *this;
    }

    const set<string> &getG_NameFilter() const {
        return g_NameFilter;
    }

    bool isEmpty()const{ return g_Empty; }

    bool filter(const CStudent & p_student)const{

        if(g_Empty)
            return true;

        return !((g_EnrolledBefore>-1 && p_student.getEnrolled() >= g_EnrolledBefore) ||
                         (g_EnrolledAfter>-1 && p_student.getEnrolled() <= g_EnrolledAfter) ||
                         (dbOn && g_BornBefore->CompareTo(p_student.getBirthDate())<0) ||
                         (daOn && g_BornAfter->CompareTo(p_student.getBirthDate())>0));

    }

private:
    bool g_Empty;
    set<string> g_NameFilter;
    bool dbOn;
    bool daOn;
    CDate * g_BornBefore;
    CDate * g_BornAfter;
    int g_EnrolledBefore;
    int g_EnrolledAfter;
};

class CSort {
public:

    CSort(void){}

     CSort& AddKey (ESortKey key, bool ascending = true) {
         keyList.push_back({key,ascending});
         return *this;
     }

    vector<pair<ESortKey , bool>> keyList;
};


class StudentComparator{
public:

    StudentComparator():g_Options(){}

    StudentComparator(const CSort & p_Sort):g_Options(p_Sort){}

    bool operator() (const CStudent & s1, const CStudent & s2)const{
            for (auto & l_SortOption : g_Options.keyList) {
                switch (l_SortOption.first) {
                    case ESortKey ::NAME:
                        if (s1.getFullName().compare(s2.getFullName())) {
                            if (l_SortOption.second ? s1.getFullName() < s2.getFullName()
                                                    : s1.getFullName() > s2.getFullName())
                                return true;
                        } else
                            continue;
                        break;
                    case ESortKey ::BIRTH_DATE:
                        if (s1.getBirthDate().CompareTo(s2.getBirthDate())) {
                            if (l_SortOption.second ? s1.getBirthDate().CompareTo(s2.getBirthDate()) < 0
                                                    : s2.getBirthDate().CompareTo(s1.getBirthDate()) < 0)
                                return true;
                        } else
                            continue;
                        break;
                    case ESortKey ::ENROLL_YEAR:
                        if(s1.getEnrolled()!=s2.getEnrolled()) {
                            if (l_SortOption.second ? s1.getEnrolled() < s2.getEnrolled()
                                                    : s2.getEnrolled() < s1.getEnrolled())
                                return true;
                        }else
                            continue;
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
    CStudyDept(void):g_StudentCnt(0){}

    bool AddStudent(const CStudent &x){

        CStudent s = x;
        s.setOrder(++g_StudentCnt);

        bool succ = g_Students.insert(s).second;

        if(succ) {
            shared_ptr<CStudent> shp(new CStudent(x.getFullName(), x.getBirthDate(), x.getEnrolled()));
            string key = s.getKey();
            g_ByNicks[key].insert(s);
            fillSuggestMap(key,shp);
        }
        return succ;
    }

    bool DelStudent(const CStudent &x){

        if(!g_Students.count(x))
            return false;

        g_ByNicks.at(x.getKey()).erase(x);

        g_Students.erase(x);
        return true;
    }

    list<CStudent> Search(const CFilter &flt,
                          const CSort &sortOpt) const{

        list<CStudent> l_List;

            if(flt.getG_NameFilter().empty())
                for (const CStudent & s : g_Students) {
                    if(flt.filter(s))
                        l_List.push_back(s);
                }
            else {
                for (const string & s : flt.getG_NameFilter()) {
                    try {
                        set<CStudent> it = g_ByNicks.at(s);
                        for (const CStudent & st : it)
                            if (flt.filter(st))
                                l_List.push_back(st);
                    }catch (const std::exception& ){
                        continue;
                    }


                }
            }


        l_List.sort(StudentComparator(sortOpt));

        return l_List;
    }

    void fillSuggestMap(string name,shared_ptr<CStudent> student) {

        string l_WordPart;

        for (size_t i = 0; i < name.length(); i++) {
            if (!l_WordPart.empty() && name[i] == ' ') {
                g_SuggestMap[l_WordPart].insert(student);
                l_WordPart.clear();
            } else {
                l_WordPart += name[i];
            }
        }

        if (!l_WordPart.empty())
            g_SuggestMap[l_WordPart].insert(student);
    }

    set<string> Suggest(const string &name) const{
        set<shared_ptr<CStudent>> l_Students;
        set<string> result;
        vector<string> l_names;
        string cp_name = name;
        std::transform(name.begin(),name.end(),cp_name.begin(),::tolower);
        split(cp_name,l_names);
        for(const string & n : l_names){
            try {
                set<shared_ptr<CStudent>> m = g_SuggestMap.at(n), intersect;
                if (l_Students.empty()) {
                    l_Students = m;
                    //cout << "ascadscAS" << endl;
                } else {
                    set_intersection(l_Students.begin(), l_Students.end(), m.begin(), m.end(),
                                     std::inserter(intersect, intersect.begin()));
                    if(intersect.empty())
                        return result;
                }
                if (!intersect.empty())
                    l_Students = intersect;
            }catch (const exception&){
                return result;
            }
        }

        for(const auto st : l_Students)
            result.insert(st->getFullName());

        return result;
    }

private:
    uint64_t g_StudentCnt;
    set<CStudent> g_Students;
    map<string,set<CStudent>> g_ByNicks;
    map<string,set<shared_ptr<CStudent>>> g_SuggestMap;
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
    assert (!x0.AddStudent(CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014)));
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

    assert (x0.Search(CFilter().Name("james bond").Name("bond james"),
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

    //revision 3

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

    // revison 3

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