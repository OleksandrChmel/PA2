#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

// enable only if your implementation supports Add ( ) with more than three parameters
// #define MULTIPLE_STOPS

template <class T,class E>
class Node{
public:
    Node(const T & next,const E & con):nextNode(next),connection(con){}

    bool operator < (const Node<T,E> & other)const {
        return nextNode<other.nextNode;
    }

    T nextNode;
    E connection;

};

template <typename _T, typename _E>
class CAccess
{
    std::map<_T,set<Node<_T,_E>>> g_grafMap;
    set<_T> g_station_list;

public:
    CAccess(){}
    // destructor
    CAccess & Add(const _E & p_obj,const _T & p_Node1,const _T & p_Node2){
        g_grafMap[p_Node1].insert(Node<_T,_E>(p_Node2,p_obj));
        g_grafMap[p_Node2].insert(Node<_T,_E>(p_Node1,p_obj));
        g_station_list.insert(p_Node1);
        g_station_list.insert(p_Node2);
        return * this;
    }

    int Find ( const _T & src, const _T & dst ) const
    {
        std::queue < std::pair < _T, unsigned > > q;
        std::set < _T > visited;
        std::map < _T, _T > pred;

        q.push({src,0});
        visited.insert ( src );

        while ( ! q.empty ( ) )
        {
            // vyndej z fronty
            const _T cnode = q.front ( ).first;
            const unsigned cdist = q.front ( ).second;
            q.pop ( );

            if ( cnode == dst )
            {
                // vypis cestu pozpatku
                std::cout << dst;// << ", ";
                _T tmp = dst;

                while ( 1 )
                {
                    if ( tmp == src ) break;
                    std::cout <<  ", " << pred [ tmp ];
                    tmp = pred [ tmp ];
                }
                std::cout << std::endl;

                return cdist;
            }
                for (const Node<_T, _E> &n : g_grafMap.at(cnode)) {
                    if (visited.count(n.nextNode) == 0) {
                        q.push(std::make_pair(n.nextNode, cdist + 1));
                        visited.insert(n.nextNode);
                        pred.insert(std::make_pair(n.nextNode, cnode));
                    }
                }
        }

        return -1;
    }

    map<_T,int> Find(const _T & p_startPoint,int p_Max = -1){
        map<_T,int > l_Lines;

        if(g_station_list.count(p_startPoint)==0)
            throw invalid_argument("unknown "+p_startPoint);

        for (const _T & NODE : g_station_list) {
            int distance = Find(p_startPoint, NODE);
            if(distance>=0) {
                if (p_Max != -1 && distance > p_Max)
                    continue;
                l_Lines.insert({NODE, distance});
            }
        }

        return l_Lines;
    }


};

template <class  T1, class  T2>
bool operator < (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs){
    return lhs.second<rhs.second;
};

#ifndef __PROGTEST__
//=================================================================================================
class CTrain
{
public:
    CTrain                        ( const string    & company,
                                    int               speed )
            : m_Company ( company ),
              m_Speed ( speed )
    {
    }
    //---------------------------------------------------------------------------------------------
    string                   m_Company;
    int                      m_Speed;
};
//=================================================================================================
class TrainFilterCompany
{
public:
    TrainFilterCompany            ( const set<string> & companies )
            : m_Companies ( companies )
    {
    }
    //---------------------------------------------------------------------------------------------
    bool                     operator ()                   ( const CTrain & train ) const
    {
        return m_Companies . find ( train . m_Company ) != m_Companies . end ();
    }
    //---------------------------------------------------------------------------------------------
private:
    set <string>             m_Companies;
};
//=================================================================================================
class TrainFilterSpeed
{
public:
    TrainFilterSpeed              ( int               min,
                                    int               max )
            : m_Min ( min ),
              m_Max ( max )
    {
    }
    //---------------------------------------------------------------------------------------------
    bool                     operator ()                   ( const CTrain    & train ) const
    {
        return train . m_Speed >= m_Min && train . m_Speed <= m_Max;
    }
    //---------------------------------------------------------------------------------------------
private:
    int                      m_Min;
    int                      m_Max;
};
//=================================================================================================
bool                         NurSchnellzug                 ( const CTrain    & zug )
{
    return ( zug . m_Company == "OBB" || zug . m_Company == "DB" ) && zug . m_Speed > 100;
}
//=================================================================================================
int                          main                          ( void )
{
    CAccess<string,CTrain> lines;
    lines . Add ( CTrain (   "DB", 120 ), "Berlin", "Prague"   )
            . Add ( CTrain (   "CD",  80 ), "Berlin", "Prague"   )
            . Add ( CTrain (   "DB", 160 ), "Berlin", "Dresden"  )
            . Add ( CTrain (   "DB", 160 ), "Dresden", "Munchen" )
            . Add ( CTrain (   "CD",  90 ), "Munchen", "Prague"  )
            . Add ( CTrain (   "DB", 200 ), "Munchen", "Linz"    )
            . Add ( CTrain (  "OBB",  90 ), "Munchen", "Linz"    )
            . Add ( CTrain (   "CD",  50 ), "Linz", "Prague"     )
            . Add ( CTrain (   "CD", 100 ), "Prague", "Wien"     )
            . Add ( CTrain (  "OBB", 160 ), "Linz", "Wien"       )
            . Add ( CTrain ( "SNCF", 300 ), "Paris", "Marseille" )
            . Add ( CTrain ( "SNCF", 250 ), "Paris", "Dresden"   )
            . Add ( CTrain ( "SNCF", 200 ), "London", "Calais"   );
    assert ( lines . Find ( "Berlin" ) == (map<string,int>
            {
                    make_pair ( "Berlin", 0 ),
                    make_pair ( "Dresden", 1 ),
                    make_pair ( "Linz", 2 ),
                    make_pair ( "Marseille", 3 ),
                    make_pair ( "Munchen", 2 ),
                    make_pair ( "Paris", 2 ),
                    make_pair ( "Prague", 1 ),
                    make_pair ( "Wien", 2 )
            }) );
    assert ( lines . Find ( "London" ) == (map<string,int>
            {
                    make_pair ( "Calais", 1 ),
                    make_pair ( "London", 0 )
            }) );
    assert ( lines . Find ( "Wien" ) == (map<string,int>
            {
                    make_pair ( "Berlin", 2 ),
                    make_pair ( "Dresden", 3 ),
                    make_pair ( "Linz", 1 ),
                    make_pair ( "Marseille", 5 ),
                    make_pair ( "Munchen", 2 ),
                    make_pair ( "Paris", 4 ),
                    make_pair ( "Prague", 1 ),
                    make_pair ( "Wien", 0 )
            }) );
    assert ( lines . Find ( "Wien", 3 ) == (map<string,int>
            {
                    make_pair ( "Berlin", 2 ),
                    make_pair ( "Dresden", 3 ),
                    make_pair ( "Linz", 1 ),
                    make_pair ( "Munchen", 2 ),
                    make_pair ( "Prague", 1 ),
                    make_pair ( "Wien", 0 )
            }) );

    //revision1

    /*assert ( lines . Find ( "Wien", 5, NurSchnellzug ) == (map<string,int>
            {
                    make_pair ( "Berlin", 4 ),
                    make_pair ( "Dresden", 3 ),
                    make_pair ( "Linz", 1 ),
                    make_pair ( "Munchen", 2 ),
                    make_pair ( "Prague", 5 ),
                    make_pair ( "Wien", 0 )
            }) );
    /*assert ( lines . Find ( "Prague", 3,  TrainFilterCompany ( { "CD", "DB" } ) ) == (map<string,int>
            {
                    make_pair ( "Berlin", 1 ),
                    make_pair ( "Dresden", 2 ),
                    make_pair ( "Linz", 1 ),
                    make_pair ( "Munchen", 1 ),
                    make_pair ( "Prague", 0 ),
                    make_pair ( "Wien", 1 )
            }) );
    assert ( lines . Find ( "Munchen", 4, TrainFilterSpeed ( 160, 250 ) ) == (map<string,int>
            {
                    make_pair ( "Berlin", 2 ),
                    make_pair ( "Dresden", 1 ),
                    make_pair ( "Linz", 1 ),
                    make_pair ( "Munchen", 0 ),
                    make_pair ( "Paris", 2 ),
                    make_pair ( "Wien", 2 )
            }) );
    assert ( lines . Find ( "Munchen", 4, [] ( const CTrain & x ) { return x . m_Company == "CD"; } ) == (map<string,int>
            {
                    make_pair ( "Berlin", 2 ),
                    make_pair ( "Linz", 2 ),
                    make_pair ( "Munchen", 0 ),
                    make_pair ( "Prague", 1 ),
                    make_pair ( "Wien", 2 )
            }) );
    assert ( lines . Find ( "London", 20, [] ( const CTrain & x ) { return x . m_Company == "CD"; } ) == (map<string,int>
            {
                    make_pair ( "London", 0 )
            }) );*/
    try
    {
        auto res = lines . Find ( "Salzburg" );
        assert ( "No exception thrown" == NULL );
    }
    catch ( const invalid_argument & e )
    {
        assert ( string ( e . what () ) == "unknown Salzburg" );
    }
#ifdef MULTIPLE_STOPS
    lines . Add ( CTrain ( "RZD",  80 ), "Prague", "Kiev", "Moscow", "Omsk", "Irkutsk", "Vladivostok" );
  lines . Add ( CTrain ( "LAV", 160 ), "Malaga", "Cordoba", "Toledo", "Madrid", "Zaragoza", "Tarragona", "Barcelona", "Montpellier", "Marseille" );
  assert ( lines . Find ( "Madrid" ) == (map<string,int>
  {
    make_pair ( "Barcelona", 1 ),
    make_pair ( "Berlin", 4 ),
    make_pair ( "Cordoba", 1 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 6 ),
    make_pair ( "Kiev", 6 ),
    make_pair ( "Linz", 5 ),
    make_pair ( "Madrid", 0 ),
    make_pair ( "Malaga", 1 ),
    make_pair ( "Marseille", 1 ),
    make_pair ( "Montpellier", 1 ),
    make_pair ( "Moscow", 6 ),
    make_pair ( "Munchen", 4 ),
    make_pair ( "Omsk", 6 ),
    make_pair ( "Paris", 2 ),
    make_pair ( "Prague", 5 ),
    make_pair ( "Tarragona", 1 ),
    make_pair ( "Toledo", 1 ),
    make_pair ( "Vladivostok", 6 ),
    make_pair ( "Wien", 6 ),
    make_pair ( "Zaragoza", 1 )
  }) );
  assert ( lines . Find ( "Omsk", 4 ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 1 ),
    make_pair ( "Kiev", 1 ),
    make_pair ( "Linz", 2 ),
    make_pair ( "Moscow", 1 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Omsk", 0 ),
    make_pair ( "Paris", 4 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Vladivostok", 1 ),
    make_pair ( "Wien", 2 )
  }) );
  assert ( lines . Find ( "Moscow", 10, TrainFilterCompany ( { "RZD", "DB" } ) ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 1 ),
    make_pair ( "Kiev", 1 ),
    make_pair ( "Linz", 5 ),
    make_pair ( "Moscow", 0 ),
    make_pair ( "Munchen", 4 ),
    make_pair ( "Omsk", 1 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Vladivostok", 1 )
  }) );
#endif /* MULTIPLE_STOPS */
    return 0;
}
#endif  /* __PROGTEST__ */