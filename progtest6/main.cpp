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
#define MULTIPLE_STOPS
/*
 * A simple C++ application that allows to user cerate a map of different objects,
 * for example raod map, computer network or air connection between cities and quickly
 * find distance between two points that will answer a specific filter
 *
 * class Node
 *
 * class CAccess
 * */

//===============================================class Node=============================================================

/*Represents a templated unit of map
 * */
template<class T, class E>
class Node {
public:
    /*standart constructor
     *
     * @param p_node An object that will be stored in Node
     * @param con Object that represents connection between this a neighbors
     * */
    Node(const T &p_node,E &con) : g_NODE(p_node){
        g_Connections.push_back(con);
    }

    /*operator < that is used while inserting Node to STL MAP
     *
     * @param other Another Node which will be compared with this
     *
     * @return TRUE if other bigger then this
     *         FALSE opposite situation
     * */
    bool operator<(const Node<T, E> &other) const {
        return g_NODE < other.g_NODE;
    }
//----------------------------------------------------------------------------------------------------------------------
    T g_NODE;
    mutable vector<E> g_Connections;

};

//=============================================class Node End===========================================================


//==============================================class CAccess===========================================================

/*Represents a container that stores a map of Nodes. Use BFS search to find shortest abstract distance between
 * two Nodes. Allows to find all possible connection between StartPoint and all other Nodes stored in map using filter
 * */
template<typename _T, typename _E>
class CAccess {
private:

    std::map<_T, set<Node<_T, _E>>> g_grafMap;
    set<_T> g_station_list;

public:

    template<typename...Nodes>

    CAccess & Add (const _E& p_Edge, const _T& p_Node1, const _T& p_Node2, Nodes...p_Nodes)
    {
        Add(p_Edge,p_Node1,p_Node2);
        Add(p_Edge,p_Node1,p_Nodes...);
        Add(p_Edge,p_Node2,p_Nodes...);
        return *this;
    }

/*Add two Nodes to the map and all connections between them
 *
 * @param p_Connection Object that represents connection between two Nodes
 * @param p_Node1 Object Node
 * @param p_Node2 Object Node
 *
 * @return reference on class CAccess
 * */
    CAccess &Add(const _E &p_Connection, const _T &p_Node1, const _T &p_Node2) {
        _E l_Connection(p_Connection);

        bool l_SuccessOnInsert = g_grafMap[p_Node2].insert(Node<_T, _E>(p_Node1, l_Connection)).second;

        //if Node was added before push_back next Connection
        if(!l_SuccessOnInsert)
           g_grafMap[p_Node1].find(Node<_T, _E>(p_Node2, l_Connection))->g_Connections.push_back(l_Connection);

        l_SuccessOnInsert = g_grafMap[p_Node1].insert(Node<_T, _E>(p_Node2, l_Connection)).second;

        //if Node was added before push_back next Connection
        if(!l_SuccessOnInsert)
            g_grafMap[p_Node2].find(Node<_T, _E>(p_Node1, l_Connection))->g_Connections.push_back(l_Connection);

        //Add Node to the Nodes list
        g_station_list.insert(p_Node1);
        g_station_list.insert(p_Node2);
        return *this;
    }

    /*Finds all reachable other Nodes and abstract distance between them
     *
     * @param p_StartNode A Node to start search from
     * @param p_Max Max distance between StartNode and Destination
     * @param p_functionCompare Filters result
     *
     * @return MAP of reachable Nodes as key and distance from StartPoint as value
     *         EMPTY MAP if nothing was found
     * */

    const map<_T, int> Find(const _T &p_startPoint,const int p_Max = -1,const std::function<bool(const _E &)> p_functionCompare = [](const _E & other){ return true;}) const {

        map<_T, int> l_Lines;

        //if Node doesn't exists throw an exception
        if (g_station_list.count(p_startPoint) == 0) {
            ostringstream l_stream;
            l_stream << p_startPoint;
            throw invalid_argument("unknown " + l_stream.str());
        }

        //Foreach Node get abstract distance from Node p_StartPoint to Node NODE
        std::queue<std::pair<_T, unsigned> > l_NodeQueue;
        std::set<_T> l_NodesVisited;

        l_Lines.insert({p_startPoint,0});
        l_NodeQueue.push({p_startPoint, 0});
        l_NodesVisited.insert(p_startPoint);

        //while some nodes still reachable
        while (!l_NodeQueue.empty()) {
            const _T l_CurrentNode = l_NodeQueue.front().first;
            const unsigned l_TotalDistance = l_NodeQueue.front().second;
            l_NodeQueue.pop();

            for (const Node<_T, _E> &l_NODE : g_grafMap.at(l_CurrentNode)) {
                //if not visited and match the filter
                if (l_NodesVisited.count(l_NODE.g_NODE) == 0 && Match(l_NODE.g_Connections,p_functionCompare) && (p_Max==-1 || l_TotalDistance<(uint64_t)p_Max)) {
                    l_Lines.insert({l_NODE.g_NODE,l_TotalDistance+1});
                    l_NodeQueue.push(std::make_pair(l_NODE.g_NODE, l_TotalDistance + 1));
                    l_NodesVisited.insert(l_NODE.g_NODE);
                }
            }
        }
        return l_Lines;
    }

    /*Check all possible connections from Node to Node
     *
     * @param p_Connections Container that represents set of possible connections
     * @param p_functionCompare Filters the set of connections
     *
     * @return TRUE on success if at least one connection match the filter
     *         FALSE in no of them
     * */

    bool Match(const vector<_E> & p_Connections,const std::function<bool(const _E &)> p_functionCompare)const{
        //Loop through the Connections
        for (const _E & e : p_Connections)
            if(p_functionCompare(e))
                return true;

        //on unsuccess
        return false;
    }
};

//===============================================class CAccess End======================================================

#ifndef __PROGTEST__

//=================================================================================================
class CTrain {
public:
    CTrain(const string &company,
           int speed)
            : m_Company(company),
              m_Speed(speed) {
    }

    //---------------------------------------------------------------------------------------------
    string m_Company;
    int m_Speed;
};

//=================================================================================================
class TrainFilterCompany {
public:
    TrainFilterCompany(const set<string> &companies)
            : m_Companies(companies) {
    }

    //---------------------------------------------------------------------------------------------
    bool operator()(const CTrain &train) const {
        return m_Companies.find(train.m_Company) != m_Companies.end();
    }
    //---------------------------------------------------------------------------------------------
private:
    set<string> m_Companies;
};

//=================================================================================================
class TrainFilterSpeed {
public:
    TrainFilterSpeed(int min,
                     int max)
            : m_Min(min),
              m_Max(max) {
    }

    //---------------------------------------------------------------------------------------------
    bool operator()(const CTrain &train) const {
        return train.m_Speed >= m_Min && train.m_Speed <= m_Max;
    }
    //---------------------------------------------------------------------------------------------
private:
    int m_Min;
    int m_Max;
};

//=================================================================================================
bool NurSchnellzug(const CTrain &zug) {
    return (zug.m_Company == "OBB" || zug.m_Company == "DB") && zug.m_Speed > 100;
}

//=================================================================================================
int main(void) {
    CAccess<string, CTrain> lines;
    lines.Add(CTrain("DB", 120), "Berlin", "Prague")
            .Add(CTrain("CD", 80), "Berlin", "Prague")
            .Add(CTrain("DB", 160), "Berlin", "Dresden")
            .Add(CTrain("DB", 160), "Dresden", "Munchen")
            .Add(CTrain("CD", 90), "Munchen", "Prague")
            .Add(CTrain("DB", 200), "Munchen", "Linz")
            .Add(CTrain("OBB", 90), "Munchen", "Linz")
            .Add(CTrain("CD", 50), "Linz", "Prague")
            .Add(CTrain("CD", 100), "Prague", "Wien")
            .Add(CTrain("OBB", 160), "Linz", "Wien")
            .Add(CTrain("SNCF", 300), "Paris", "Marseille")
            .Add(CTrain("SNCF", 250), "Paris", "Dresden")
            .Add(CTrain("SNCF", 200), "London", "Calais");
    assert (lines.Find("Berlin") == (map<string, int>
            {
                    make_pair("Berlin", 0),
                    make_pair("Dresden", 1),
                    make_pair("Linz", 2),
                    make_pair("Marseille", 3),
                    make_pair("Munchen", 2),
                    make_pair("Paris", 2),
                    make_pair("Prague", 1),
                    make_pair("Wien", 2)
            }));
    assert (lines.Find("London") == (map<string, int>
            {
                    make_pair("Calais", 1),
                    make_pair("London", 0)
            }));
    assert (lines.Find("Wien") == (map<string, int>
            {
                    make_pair("Berlin", 2),
                    make_pair("Dresden", 3),
                    make_pair("Linz", 1),
                    make_pair("Marseille", 5),
                    make_pair("Munchen", 2),
                    make_pair("Paris", 4),
                    make_pair("Prague", 1),
                    make_pair("Wien", 0)
            }));
    assert (lines.Find("Wien", 3) == (map<string, int>
            {
                    make_pair("Berlin", 2),
                    make_pair("Dresden", 3),
                    make_pair("Linz", 1),
                    make_pair("Munchen", 2),
                    make_pair("Prague", 1),
                    make_pair("Wien", 0)
            }));

    //revision1

    assert (lines.Find("Wien", 5, NurSchnellzug) == (map<string, int>
            {
                    make_pair("Berlin", 4),
                    make_pair("Dresden", 3),
                    make_pair("Linz", 1),
                    make_pair("Munchen", 2),
                    make_pair("Prague", 5),
                    make_pair("Wien", 0)
            }));
    assert ( lines . Find ( "Prague", 3,  TrainFilterCompany ( { "CD", "DB" } ) ) == (map<string,int>
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
    assert( lines . Find ( "Munchen", 4, [] ( const CTrain & x ) { return x . m_Company == "CD"; } ) == (map<string,int>
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
            }) );
    try {
        auto res = lines.Find("Salzburg");
        assert ("No exception thrown" == NULL);
    }
    catch (const invalid_argument &e) {
        assert (string(e.what()) == "unknown Salzburg");
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