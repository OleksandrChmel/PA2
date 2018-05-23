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
#include <numeric>

using namespace std;

class CDate
{
public:
    CDate (int y, int m, int d)
            : m_Y (y), m_M (m), m_D (d)
    {}

    int CompareTo (const CDate & other) const
    {
        if (m_Y != other.m_Y)
            return (other.m_Y < m_Y) - (m_Y < other.m_Y);
        if (m_M != other.m_M)
            return (other.m_M < m_M) - (m_M < other.m_M);
        return (other.m_D < m_D) - (m_D < other.m_D);
    }

    friend ostream & operator << (ostream & os, const CDate & d)
    {
        return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
    }

private:
    int m_Y;
    int m_M;
    int m_D;
};

enum class ESortKey
{
    NAME,
    BIRTH_DATE,
    ENROLL_YEAR
};

#endif /* __PROGTEST__ */
/**
 * Class represents student with name, date of birth, enrolled year
 * and also keeps the registration number in m_id.
 */
class CStudent
{
public:
    CStudent (const string & name, const CDate & born, int enrolled)
            :m_name(name), m_born(born), m_enrolled(enrolled)
    {}

    bool operator == (const CStudent & other) const
    {
        if (m_name == other.m_name && m_born.CompareTo(other.m_born) == 0 && m_enrolled == other.m_enrolled)
            return true;
        return false;
    }

    bool operator != (const CStudent & other) const
    {
        if (m_name != other.m_name || m_born.CompareTo(other.m_born) != 0 || m_enrolled != other.m_enrolled)
            return true;
        return false;
    }

    bool operator < (const CStudent & x1) const
    {
        if (m_name < x1.m_name)
            return true;
        if (m_name == x1.m_name && m_born.CompareTo(x1.m_born) != 0)
            return true;
        if (m_name == x1.m_name &&  m_born.CompareTo(x1.m_born) == 0 && m_enrolled < x1.m_enrolled)
            return true;
        return false;
    }

    //getter and setters generated automatically
    const string &getM_name() const
    {
        return m_name;
    }

    const CDate &getM_born() const
    {
        return m_born;
    }

    int getM_enrolled() const
    {
        return m_enrolled;
    }

    int getM_id() const
    {
        return m_id;
    }

    void setM_id(int m_id)
    {
        CStudent::m_id = m_id;
    }

    friend ostream & operator << (ostream & os, const CStudent & d)
    {
        return os << d.getM_name() << '-' << d.getM_born() << '-' << d.getM_enrolled();
    }

private:
    string m_name;
    CDate m_born;
    int m_enrolled;
    int m_id;
};

/**
 * Function separates string by whitespace delimeter.
 * Makes a string from the whole name, so it can be used as a key to find names.
 * @param p_name string
 * @return string n_key
 */
string SplitName (string p_name)
{
    vector<string> name_vector;
    string s_name;
    string s_key;
    transform(p_name.begin(), p_name.end(), p_name.begin(), ::tolower);
    int position = 0;
    for (unsigned int i = 0; i <= p_name.size(); i++)
    {
        if (p_name[i] == ' ' || i == p_name.size())
        {
            s_name+=" ";
            name_vector.push_back(s_name);
            s_name.resize(0);
            position = 0;
            continue;
        }
        s_name.resize(s_name.size() + 1);
        s_name[position] += p_name[i];
        position++;
    }
    sort (name_vector.begin(), name_vector.end());
    s_key = accumulate(name_vector.begin(), name_vector.end(), s_key);
    return s_key;
}

/**
 * Function separates string by whitespace delimeter.
 * Makes a vector of name parts separated.
 * @param p_name string name
 * @return vector name_vector_2
 */
vector<string> SplitNameV (string p_name)
{
    vector<string> name_vector_2;
    string s_name;
    int position = 0;
    transform(p_name.begin(), p_name.end(), p_name.begin(), ::tolower);
    for (unsigned int i = 0; i <= p_name.size(); i++)
    {
        if (p_name[i] == ' ' || i == p_name.size())
        {
            name_vector_2.push_back(s_name);
            s_name.resize(0);
            position = 0;
            continue;
        }
        s_name.resize(s_name.size() + 1);
        s_name[position] += p_name[i];
        position++;
    }
    return name_vector_2;
}

/**
 * Class holds booleans for methods, set on true when method is called.
 * Also keeps parameters of methods.
 */
class CFilter
{
public:
    CFilter (void)
            : empty(true), born_before(false), born_after(false), enrolled_before(false), enrolled_after(false),
              d_born_before(0,0,0), d_born_after(0,0,0), y_enrolled_before(0), y_enrolled_after(0)
    {}

    CFilter & Name (const string & name)
    {
        empty = false;
        name_key_set.insert(SplitName(name));
        return *this;
    }

    CFilter & BornBefore (const CDate & date)
    {
        empty = false;
        born_before = true;
        d_born_before = date;
        return *this;
    }

    CFilter & BornAfter (const CDate & date)
    {
        empty = false;
        born_after = true;
        d_born_after = date;
        return *this;
    }

    CFilter & EnrolledBefore (int year)
    {
        empty = false;
        enrolled_before = true;
        y_enrolled_before = year;
        return *this;
    }

    CFilter & EnrolledAfter  (int year)
    {
        empty = false;
        enrolled_after = true;
        y_enrolled_after = year;
        return *this;
    }

    bool Filter (const CStudent & s1) const;


    bool empty;
    set<string> name_key_set;
private:
    bool born_before;
    bool born_after;
    bool enrolled_before;
    bool enrolled_after;
    CDate d_born_before;
    CDate d_born_after;
    int y_enrolled_before;
    int y_enrolled_after;
};

/**
     * Method decides which filter to use.
     * @param s1 is instance of CStudent
     * @return true if conditions are fulfilled to later push back to list, otherwise false
     */
bool CFilter::Filter (const CStudent & s1) const
{
    if (empty)
        return true;

    if (born_before && d_born_before.CompareTo(s1.getM_born()) <= 0)
        return false;

    if (born_after && d_born_after.CompareTo(s1.getM_born()) >= 0)
        return false;

    if (enrolled_before && s1.getM_enrolled() >= y_enrolled_before)
        return false;

    if (enrolled_after && s1.getM_enrolled() <= y_enrolled_after)
        return false;

    return true;
}

/**
 * Class keeps vector of pair that will help us decide the way of sorting.
 */
class CSort
{
public:
    CSort (void)
            : empty(true)
    {}

    CSort & AddKey (ESortKey key, bool ascending)
    {
        empty = false;
        keyList.push_back({key, ascending});
        return *this;
    }

    bool empty;
    vector<pair<ESortKey, bool>> keyList;
private:
};

/**
 * Class is used to choose way of sorting.
 */
class StudentCompare
{
public:
    StudentCompare (const CSort & opt)
            : sortOptions(opt)
    {}

    bool operator()(const CStudent & a, const CStudent & b) const;

    CSort sortOptions;
private:
};

/**
 * Method of overloading operator () to choose the way of sorting.
 * @param a instance of CStudent
 * @param b instance of CStudent
 * @return returns sorted objects depending on case, otherwise sorts by registration order.
 */
bool StudentCompare::operator()(const CStudent & a , const CStudent & b) const
{
    for (const auto & it : sortOptions.keyList)
    {
        switch (it.first)
        {
            case ESortKey::NAME:
            {
                if (a.getM_name() == b.getM_name())
                    continue;
                if (it.second)
                    return a.getM_name() < b.getM_name();
                if (!it.second)
                    return b.getM_name() < a.getM_name();
                break;
            }

            case ESortKey::BIRTH_DATE:
            {
                if (a.getM_born().CompareTo(b.getM_born()) == 0)
                    continue;
                if (it.second)
                    return a.getM_born().CompareTo(b.getM_born()) < 0;
                if (!it.second)
                    return b.getM_born().CompareTo(a.getM_born()) < 0;
                break;
            }

            case ESortKey::ENROLL_YEAR:
            {
                if (a.getM_enrolled() == b.getM_enrolled())
                    continue;
                if (it.second)
                    return a.getM_enrolled() < b.getM_enrolled();
                if (!it.second)
                    return b.getM_enrolled() < a.getM_enrolled();
                break;
            }

            default:
                break;
        }
        return false;
    }
    return a.getM_id() < b.getM_id();
}

class CStudyDept
{
public:
    CStudyDept (void)
            : counter(0)
    {}

    bool AddStudent (const CStudent & x);

    bool DelStudent (const CStudent & x);

    list<CStudent> Search (const CFilter & flt, const CSort & sortOpt) const
    {
        list<CStudent> l_List;

        if (flt.empty && sortOpt.empty)
        {
            for (const auto & itr : student_tmp)
                l_List.push_back(itr);
            return l_List;
        }

        if (flt.empty && !sortOpt.empty)
        {
            for (const auto & itr : student_tmp)
                l_List.push_back(itr);
            l_List.sort(StudentCompare(sortOpt));
            return l_List;
        }

        //in case of filtering by name
        if (!flt.name_key_set.empty())
            for(const auto & itr : flt.name_key_set)
            {
                try
                {
                    /*at returns a reference to the mapped value of the element identified with key k
                      and assign it to set */
                    set<CStudent> key_set = m_map.at(itr);
                    for (const auto & it : key_set)
                        if (flt.Filter(it))
                            l_List.push_back(it);
                }
                catch (const exception & ex)
                {
                    l_List.clear();
                    return l_List;
                }
            }

            //in case not filtering by name choose the way of filtering
        else
        {
            for (const auto & s : student_tmp)
                if (flt.Filter(s))
                    l_List.push_back(s);
        }

        l_List.sort(StudentCompare(sortOpt));

        return l_List;
    }
    set<string> Suggest ( const string & name ) const
    {
        vector <string> suggest_vector;
        set<shared_ptr<CStudent>> shp_names;
        set <string> result;

        suggest_vector = (SplitNameV(name));

        if (shp_names.empty())
        {
            try
            {
                for (const auto & itr : suggest_vector)
                    shp_names = suggest_map.at(itr);
            }
            catch (exception & ex)
            {
                return result;
            }

        }

        if (!shp_names.empty())
        {
            try
            {
                for (const auto & itr : suggest_vector)
                {
                    set<shared_ptr<CStudent>> shp_names_tmp;
                    /*Constructs a sorted range beginning at d_first consisting of elements that are found
                     * in both sorted ranges [first1, last1) and [first2, last2).
                     * (from cppreference.com)*/
                    set_intersection(shp_names.begin(), shp_names.end(), suggest_map.at(itr).begin(),
                                     suggest_map.at(itr).end(), std::inserter(shp_names_tmp, shp_names_tmp.begin()));
                    shp_names = shp_names_tmp;
                }
            }
            catch (exception & ex)
            {
                return result;
            }

        }
        for (const auto & itr : shp_names)
            result.insert(itr->getM_name());

        return result;
    }

    map<string,set<CStudent>> m_map;
    map<string,set<shared_ptr<CStudent>>> suggest_map;
private:
    set <CStudent> student;
    vector <CStudent> student_tmp;
    int counter;
};

/**
 * Method to add student to database.
 * @param x instance of CStudent
 * @return true if student was added, false if not because one already existed
 */
bool CStudyDept::AddStudent (const CStudent & x)
{
    bool added;

    CStudent new_student = CStudent (x.getM_name(), x.getM_born(), x.getM_enrolled());
    new_student.setM_id(++counter);

    //second is set to true if a new element was inserted or false if an equivalent element already existed.
    added = student.insert(new_student).second;
    if(!added)
        return false;

    student.emplace(new_student);
    student_tmp.push_back(new_student);

    //split name and insert key into map along with the student
    m_map[SplitName(x.getM_name())].insert(new_student);

    //shared_ptr retains shared ownership of an object through a pointer
    shared_ptr<CStudent> pointer_vector (new CStudent (x.getM_name(), x.getM_born(), x.getM_enrolled()));

    for (const auto & itr : SplitNameV(x.getM_name()))
        suggest_map[itr].insert(pointer_vector);

    return true;
}

/**
 * Method deletes student from database
 * @param x instance of CStudent
 * @return true if student was deleted, false if not because not found
 */
bool CStudyDept::DelStudent (const CStudent & x)
{
    if(student.find(x) == student.end())
        return false;

    student.erase(x);
    m_map.at(SplitName(x.getM_name())).erase(x);
    return true;
}

#ifndef __PROGTEST__
int main ( void )
{
    CStudyDept x0;
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2010 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2010 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1997, 6, 17), 2010 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1997, 6, 17), 2010 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1980, 4, 11), 2016 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1980, 4, 11), 2016 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2016 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2016 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2010 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2010 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1997, 6, 17), 2016 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1997, 6, 17), 2016 ) ) );
    assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2016 ) );
    assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2016 ) ) );
    assert ( x0 . AddStudent ( CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ) ) );
    assert ( x0 . AddStudent ( CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ) ) );
    assert ( x0 . AddStudent ( CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ) ) );
    assert ( x0 . AddStudent ( CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ) ) );
    assert ( x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
    assert ( x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ) ) );
    assert ( x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ) ) );
    assert ( x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ) ) );
    assert ( x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ) ) );
    assert ( x0 . AddStudent ( CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ) ) );
    assert(x0.Search(CFilter().BornBefore(CDate(0,0,0)),CSort())==(list<CStudent>{}));
    assert ( x0 . Search ( CFilter (), CSort () ) == (list<CStudent>
            {
                    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
                    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ),
                    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 )
            }) );
    assert ( x0 . Search ( CFilter (), CSort () . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
            {
                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
                    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
                    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
                    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 )
            }) );
    assert ( x0 . Search ( CFilter (), CSort () . AddKey ( ESortKey::NAME, false ) ) == (list<CStudent>
            {
                    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ),
                    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
                    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 )
            }) );
    assert ( x0 . Search ( CFilter (), CSort () . AddKey ( ESortKey::ENROLL_YEAR, false ) . AddKey ( ESortKey::BIRTH_DATE, false ) . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
            {
                    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
                    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
                    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 )
            }) );
    assert ( x0 . Search ( CFilter () . Name ( "james bond" ), CSort () . AddKey ( ESortKey::ENROLL_YEAR, false ) . AddKey ( ESortKey::BIRTH_DATE, false ) . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
            {
                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 )
            }) );
    assert ( x0 . Search ( CFilter () . BornAfter ( CDate ( 1980, 4, 11) ) . BornBefore ( CDate ( 1983, 7, 13) ) . Name ( "John Taylor" ) . Name ( "james BOND" ), CSort () . AddKey ( ESortKey::ENROLL_YEAR, false ) . AddKey ( ESortKey::BIRTH_DATE, false ) . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
            {
                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 )
            }) );
    assert ( x0 . Search ( CFilter () . Name ( "james" ), CSort () . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
            {
            }) );
    assert ( x0 . Suggest ( "peter" ) == (set<string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor",
                    "Peter Taylor"
            }) );
    assert ( x0 . Suggest ( "bond" ) == (set<string>
            {
                    "Bond James",
                    "James Bond"
            }) );
    assert ( x0 . Suggest ( "peter joHn" ) == (set<string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }) );
    assert ( x0 . Suggest ( "peter joHn bond" ) == (set<string>
            {
            }) );
    assert ( x0 . Suggest ( "pete" ) == (set<string>
            {
            }) );
    assert ( x0 . Suggest ( "peter joHn PETER" ) == (set<string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }) );
    assert ( ! x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
    assert ( x0 . DelStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
    assert ( x0 . Search ( CFilter () . BornAfter ( CDate ( 1980, 4, 11) ) . BornBefore ( CDate ( 1983, 7, 13) ) . Name ( "John Taylor" ) . Name ( "james BOND" ), CSort () . AddKey ( ESortKey::ENROLL_YEAR, false ) . AddKey ( ESortKey::BIRTH_DATE, false ) . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
            {
                    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
                    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
                    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
                    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 )
            }) );
    assert ( ! x0 . DelStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
    return 0;
}
#endif /* __PROGTEST__ */