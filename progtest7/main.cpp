#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>

using namespace std;

class CRect {
public:
    CRect(double x,
          double y,
          double w,
          double h)
            : m_X(x),
              m_Y(y),
              m_W(w),
              m_H(h) {
    }

    friend ostream &operator<<(ostream &os,
                               const CRect &x) {
        return os << '(' << x.m_X << ',' << x.m_Y << ',' << x.m_W << ',' << x.m_H << ')';
    }

    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};

#endif /* __PROGTEST__ */

class CElement : public CRect {
public:
    int m_id;

    CElement(int id, const CRect &position) : CRect(position), m_id(id){}

    virtual ~CElement() {}

    virtual unique_ptr<CElement> clone(const CRect& size) const = 0;
    virtual string Print (const string& space,bool pipe,bool c) const
    {
        return "";
    }
    friend ostream& operator << (ostream& os, const CElement& thing)
    {
        return os<<thing.Print("", false, false);
    }


};

class CWindow : public CRect {
public:
    CWindow(const string &title,
            const CRect &absPos) : CRect(absPos), m_title(title),m_elementcnt(0) {}

    CWindow(const CWindow &x) :  CRect(x.m_X,x.m_Y,x.m_W,x.m_H),m_title(x.m_title),m_elementcnt(x.m_elementcnt) {
        for (const pair<const int, vector<unique_ptr<CElement>>> & v : x.m_map)
            for(const unique_ptr<CElement>& e : v.second){
                m_map[v.first].push_back(e->clone(CRect(0,0,1,1)));
            }
    }

    CWindow& operator=(const CWindow &x){

        m_X=x.m_X;
        m_Y=x.m_Y;
        m_W=x.m_W;
        m_H=x.m_H;
        m_elementcnt=x.m_elementcnt;
        m_title=x.m_title;

        m_map.clear();
        for (const pair<const int, vector<unique_ptr<CElement>>> & v : x.m_map)
            for(const unique_ptr<CElement>& e : v.second){
                m_map[v.first].push_back(e->clone(CRect(0,0,1,1)));
            }

        return *this;
    }


    ~CWindow() {}


    CWindow& Add(const CElement &x)
    {
        m_map[x.m_id].push_back(x.clone(CRect(m_X,m_Y,m_W,m_H)));
        ++m_elementcnt;
        return *this;
    }

    void SetPosition (const CRect & x) {
        for (auto &it :m_map)
        {
            for(unsigned int i=0 ; i< it.second.size(); ++i)
            {
                it.second[i]->m_H=(it.second[i]->m_H/m_H)*x.m_H;
                it.second[i]->m_W=(it.second[i]->m_W/m_W)*x.m_W;
                it.second[i]->m_X=x.m_X+(it.second[i]->m_X-m_X)/m_W*x.m_W;
                it.second[i]->m_Y=x.m_Y+(it.second[i]->m_Y-m_Y)/m_H*x.m_H;
            }

        }
        m_X=x.m_X;
        m_Y=x.m_Y;
        m_W=x.m_W;
        m_H=x.m_H;
    }



    friend ostream& operator << (ostream& os, const CWindow& thing)
    {
        int a=1;
        os<< "Window " << "\"" << thing.m_title << "\"" << " (" << thing.m_X << "," << thing.m_Y << "," << thing.m_W << "," << thing.m_H << ")"<<"\n";
        for(auto & it : thing.getM_map() )
        {
            for (unsigned i = 0; i < it.second.size(); ++i)
            {
                os << "+- ";
                os << it.second[i]->Print("  ",a!=thing.m_elementcnt, true);
                a++;
                //it.second [i]->m_X=
            }
        }
        return  os;
    }

    const map<int, vector<unique_ptr<CElement>>> &getM_map() const {
        return m_map;
    }

    //Search
    unique_ptr<CElement>& Search(int id){
        try {
            return m_map.at(id)[0];
        }   catch (exception&){
            return u;
        }

    }

private:
    string m_title;
    int m_elementcnt;
    map<int, vector<unique_ptr<CElement>>> m_map;
    unique_ptr<CElement> u = nullptr;
};

class CButton : public CElement {
public:
    CButton(int id,
            const CRect &relPos,
            const string &name) : CElement(id, relPos),m_title(name) {}

    virtual ~CButton() {}

    virtual unique_ptr<CElement> clone(const CRect& size)const{
        return unique_ptr<CElement>(new CButton(m_id,CRect(size.m_X+m_X*size.m_W,size.m_Y+m_Y*size.m_H,m_W*size.m_W,m_H*size.m_H),m_title));
    }
    string Print (const string& space,bool pipe,bool c) const override {
        ostringstream os;
        os << "[" << m_id << "]"<< " Button " <<"\"" << m_title<< "\""<< " (" <<
           m_X << "," << m_Y << "," << m_W << "," << m_H << ")"<<"\n";
        return os.str();
    }

private:
    string m_title;
};

class CInput : public CElement {
private:
    string m_value;
public:
    CInput(int id,
           const CRect &relPos,
           const string &value) : CElement(id, relPos),m_value(value) {}

    virtual unique_ptr<CElement> clone(const CRect& size)const{
        return unique_ptr<CElement>(new CInput(m_id,CRect(size.m_X+m_X*size.m_W,size.m_Y+m_Y*size.m_H,m_W*size.m_W,m_H*size.m_H),m_value));
    }
    string Print (const string& space,bool pipe,bool c) const override {
        ostringstream os;
        os << "[" << m_id << "]" << " Input " << "\"" << m_value << "\"" << " ("<<
           m_X << "," << m_Y << "," << m_W << "," << m_H << ")" <<"\n";
        return os.str();
    }



    // SetValue
    const string &GetValue() const {
        return m_value;
    }

    void SetValue(const string &m_value) {
        CInput::m_value = m_value;
    }

    // GetValue

};

class CLabel : public CElement {
private:
    string m_label;
public:
    CLabel(int id,
           const CRect &relPos,
           const string &label) : CElement(id, relPos),m_label(label) {}

    virtual unique_ptr<CElement> clone(const CRect& size)const{
        return unique_ptr<CElement>(new CLabel(m_id,CRect(size.m_X+m_X*size.m_W,size.m_Y+m_Y*size.m_H,m_W*size.m_W,m_H*size.m_H),m_label));
    }
    string Print (const string& space ,bool pipe,bool c) const override {
        ostringstream os;
        os << "[" << m_id << "]" << " Label " << "\"" << m_label << "\"" <<" (" <<
           m_X << "," << m_Y << "," << m_W << "," << m_H << ")"<<"\n";
        return os.str();
    }


};

class CComboBox : public CElement {
private:
    uint64_t m_seleced;
    vector<string> m_values;
public:
    CComboBox(int id,
              const CRect &relPos,vector<string> e = {}) : CElement(id, relPos),m_seleced(0),m_values(e) {}

    CComboBox Add(const string &a) {
        m_values.push_back(a);
        return *this;
    }
    CComboBox SetSelected(const int &index = 0) {
        m_seleced=index;
        return *this;
    }

    // GetSelected

    uint64_t  GetSelected()const{
        return m_seleced;
    }

    virtual unique_ptr<CElement> clone(const CRect& size)const{
        return unique_ptr<CElement>(new CComboBox(m_id,CRect(size.m_X+m_X*size.m_W,size.m_Y+m_Y*size.m_H,m_W*size.m_W,m_H*size.m_H),m_values));
    }

    string Print (const string& space,bool pipe,bool c) const override {
        ostringstream os;
        os << "[" << m_id << "]" << " ComboBox " << "(" << m_X << "," << m_Y << "," << m_W << "," << m_H << ")"<<"\n";
        for(unsigned i=0 ; i<m_values.size();i++ )
        {
            if(i==m_seleced)
            {
                os << (c ? (pipe ? "|" : " ") : "" ) << space << "+->" << m_values.at(i)<<"<\n";
            } else
            {
                os << (c ? (pipe ? "|" : " ") : "" ) << space <<"+- " << m_values.at(i) << "\n";
            }
        }

        return os.str();
    }
};

// output operators

#ifndef __PROGTEST__

template<typename _T>
string toString(const _T &x) {
    ostringstream oss;
    oss << x;
    cout << oss.str();
    return oss.str();
}

int main(void) {
    assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );

    CWindow a("Sample window", CRect(10, 10, 600, 480));
    a.Add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).Add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
    a.Add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
    a.Add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
    a.Add(CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).Add("Karate").Add("Judo").Add("Box").Add("Progtest"));


    assert (toString(a) ==
            "Window \"Sample window\" (10,10,600,480)\n"
                    "+- [1] Button \"Ok\" (70,394,180,48)\n"
                    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
                    "+- [10] Label \"Username:\" (70,58,120,48)\n"
                    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
                    "+- [20] ComboBox (70,154,480,48)\n"
                    "   +->Karate<\n"
                    "   +- Judo\n"
                    "   +- Box\n"
                    "   +- Progtest\n");
      CWindow b= a;
      assert ( toString ( *b . Search ( 20 ) ) ==
        "[20] ComboBox (70,154,480,48)\n"
        "+->Karate<\n"
        "+- Judo\n"
        "+- Box\n"
        "+- Progtest\n" );
      assert ( dynamic_cast<CComboBox &> ( *b . Search ( 20 ) ) . GetSelected () == 0 );
      dynamic_cast<CComboBox &> ( *b . Search ( 20 ) ) . SetSelected ( 3 );
        assert ( dynamic_cast<CComboBox &> ( *b . Search ( 20 ) ) . GetSelected () == 3 );
      assert ( dynamic_cast<CInput &> ( *b . Search ( 11 ) ) . GetValue () == "chucknorris" );
      dynamic_cast<CInput &> ( *b . Search ( 11 ) ) . SetValue ( "chucknorris@fit.cvut.cz" );
      b . Add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . Add ( "PA2" ) . Add ( "OSY" ) . Add ( "Both" ) );
      assert ( toString ( b ) ==
        "Window \"Sample window\" (10,10,600,480)\n"
        "+- [1] Button \"Ok\" (70,394,180,48)\n"
        "+- [2] Button \"Cancel\" (370,394,180,48)\n"
        "+- [10] Label \"Username:\" (70,58,120,48)\n"
        "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
        "+- [20] ComboBox (70,154,480,48)\n"
        "|  +- Karate\n"
        "|  +- Judo\n"
        "|  +- Box\n"
        "|  +->Progtest<\n"
        "+- [21] ComboBox (70,250,480,48)\n"
        "   +->PA2<\n"
        "   +- OSY\n"
        "   +- Both\n" );
      assert ( toString ( a ) ==
        "Window \"Sample window\" (10,10,600,480)\n"
        "+- [1] Button \"Ok\" (70,394,180,48)\n"
        "+- [2] Button \"Cancel\" (370,394,180,48)\n"
        "+- [10] Label \"Username:\" (70,58,120,48)\n"
        "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
        "+- [20] ComboBox (70,154,480,48)\n"
        "   +->Karate<\n"
        "   +- Judo\n"
        "   +- Box\n"
        "   +- Progtest\n" );
      b . SetPosition ( CRect ( 20, 30, 640, 520 ) );
      assert ( toString ( b ) ==
        "Window \"Sample window\" (20,30,640,520)\n"
        "+- [1] Button \"Ok\" (84,446,192,52)\n"
        "+- [2] Button \"Cancel\" (404,446,192,52)\n"
        "+- [10] Label \"Username:\" (84,82,128,52)\n"
        "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
        "+- [20] ComboBox (84,186,512,52)\n"
        "|  +- Karate\n"
        "|  +- Judo\n"
        "|  +- Box\n"
        "|  +->Progtest<\n"
        "+- [21] ComboBox (84,290,512,52)\n"
        "   +->PA2<\n"
        "   +- OSY\n"
        "   +- Both\n" );
      return 0;
}

#endif /* __PROGTEST__ */