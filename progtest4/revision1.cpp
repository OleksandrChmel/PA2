#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

//---------------------------------------------------mine---------------------------------------------------------------

// class STRING
// implements functions for working with char chains

class String{
private:
    char * g_Data;
    uint64_t g_BufferLen;

public:
    String(const char * p_other){
        g_Data = new char[strlen(p_other)+1];
        strcpy(g_Data,p_other);
        g_BufferLen = strlen(p_other);
    }
    uint64_t size(){
        return g_BufferLen;
    }
    int strcmp(const char * p_other)const{
        return std::strcmp(g_Data,p_other);
    }
    const char * data()const {
        return g_Data;
    }
};

template < class T >
class c_vector
{
private:
    T       *m_Data;
    size_t   m_Size;
    size_t   m_LastIndex;
    uint16_t g_Ptr;

public:
    c_vector ( );
    //Vector& operator= ( const Vector & other );
    //Vector ( const Vector & other );
    ~c_vector ( );

    size_t size ( ) const { return m_LastIndex; }
    size_t capacity() const { return  m_Size; }
    //bool operator== ( const Vector & other ) const;
    T & operator[ ](size_t idx);
    void push_back(const T & p_Data);
    void vrealloc();
    void insert(uint64_t p_Position,const T &  p_Data);

    class Exception{};
};

template < class T >
c_vector<T>::c_vector():g_Ptr(1)
{
    m_Data = new T [ 2 ];
    m_Size = 2;
    m_LastIndex=0;

}

template <class T>
void c_vector<T>::push_back(const T & p_Data) {

    if(m_Size==m_LastIndex)
    {
        vrealloc();
    }

    m_Data[m_LastIndex++] = p_Data;

}

template <class T>
void c_vector<T>::insert(uint64_t p_Position,const T & p_Data) {

    T tmp = p_Data;
    while(p_Position<m_LastIndex)
    {
        T l_tmp  = m_Data[p_Position];
        m_Data[p_Position] = tmp;
        tmp = l_tmp;
        p_Position++;
    }
    push_back(tmp);
}

template <class T>
void c_vector<T>::vrealloc() {

    m_Size*=1.5;
    T * tmp = new T[m_Size];
    for (int i = 0; i < m_LastIndex; ++i) {
        tmp[i] = m_Data[i];
    }
    delete [] m_Data;
    m_Data = tmp;

}

template < class T >
T & c_vector<T>::operator[ ] ( size_t idx )
{
    if ( idx >= m_Size )
        throw Exception();
    return m_Data [ idx ];
}

/*template < class T >
Vector<T>::Vector ( const Vector<T> & other )
{
    copy ( other );
}
*/
/*template < class T >
Vector<T>& Vector<T>::operator= ( const Vector<T> & other )
{
    if ( this == & other ) return *this;

    delete [] m_Data;

    copy ( other );
    return *this;
}
*/
/*template < class T >
void Vector<T>::copy ( const Vector<T> & other )
{
    m_Data = new T [ other.m_Size ];
    m_Size = other.m_Size;

    for ( size_t i = 0; i < m_Size; ++ i )
        m_Data [ i ] = other.m_Data [ i ];
}
*/
template < class T >
c_vector<T>::~c_vector ( )
{
    delete [ ] m_Data;
}

class CMail
{
public:

    CMail                         ( const char      * from,
                                    const char      * to,
                                    const char      * body );
    bool                     operator ==                   ( const CMail     & x ) const;

    const String &getG_From() const;

    void setG_From(const String &g_From);

    const String &getG_To() const;

    void setG_To(const String &g_To);

    const String &getG_Body() const;

    void setG_Body(const String &g_Body);

private:
    String g_From;
    String g_To;
    String g_Body;
    uint16_t g_Ptr;
};

CMail::CMail(const char *from, const char *to, const char *body):g_From(from),g_To(to),g_Body(body),g_Ptr(1) {
}

bool CMail::operator==(const CMail &x) const {
    if(!g_From.strcmp(x.getG_From().data()) && !g_To.strcmp(x.getG_To().data()) && !g_Body.strcmp(x.getG_Body().data()))
        return true;
    else return false;
}

const String &CMail::getG_From() const {
    return g_From;
}

void CMail::setG_From(const String &g_From) {
    CMail::g_From = g_From;
}

const String &CMail::getG_To() const {
    return g_To;
}

void CMail::setG_To(const String &g_To) {
    CMail::g_To = g_To;
}

const String &CMail::getG_Body() const {
    return g_Body;
}

void CMail::setG_Body(const String &g_Body) {
    CMail::g_Body = g_Body;
}

class Container{
public:
    const CMail g_mail;
    Container * next;

    Container(const CMail & mail):g_mail(mail),next(NULL){};

};

class CMailList{
public:

    CMailList():g_begin(NULL),g_last(NULL){}

    void add(const CMail & p_Container){
        if(!g_last)
            g_begin = g_last = new Container(p_Container);
        else {
            g_last->next = new Container(p_Container);
            g_last = g_last->next;
        }
    }

    Container * begin()const {
        return g_begin;
    }

    Container * end(){
        return g_last;
    }


private:
    Container * g_begin;
    Container * g_last;
};

//------------------------------------------------CMAIL-ITERATOR--------------------------------------------------------

class CMailIterator
{
public:
    CMailIterator(Container * begin,Container * end){
        g_begin = begin;
        g_end = end;
    }
    operator bool                 ( void ) const{
        if(g_end->next==g_begin)
            return false;
         return true;
    }
    bool                     operator !                    ( void ) const{
        if(!g_begin || g_begin==g_end->next)
            return true;
        else return false;
    }
    const CMail            & operator *                    ( void ) const{
        return g_begin->g_mail;
    }
    CMailIterator          & operator ++                   ( void ){
        g_begin=g_begin->next;
        return *this;
    }
private:
    Container * g_begin;
    Container * g_end;
};

//-----------------------------------------------END-CMAIL-ITERATOR-----------------------------------------------------

// ---------------------------------------------------CLIENT------------------------------------------------------------

class Client{
public:
    Client():Inbox(),Outbox(){};
    Client(const char * username):g_username(username),Inbox(),Outbox(){}
    const char * getUsername(){ return  g_username;}
    void getMail(const CMail & p_email){
        Inbox.add(p_email);
    }
    void sendMail(const CMail & p_email){
        Outbox.add(p_email);
    }
    CMailIterator openInbox(){
        return CMailIterator(Inbox.begin(),Inbox.end());
    }
    CMailIterator openOutbox(){
        return  CMailIterator(Outbox.begin(),Outbox.end());
    }
private:
    const char * g_username;
    CMailList Inbox;
    CMailList Outbox;
};

//---------------------------------------------------END-CLIENT---------------------------------------------------------


//--------------------------------------------------CMAIL-SERVER--------------------------------------------------------
class CMailServer
{
public:
    CMailServer                   ( void );
    CMailServer                   ( const CMailServer & src );
    CMailServer            & operator =                    ( const CMailServer & src );
    //~CMailServer                  ( void );
    void                     SendMail                      ( const CMail     & m );
    CMailIterator            Outbox                        ( const char      * email );
    CMailIterator            Inbox                         ( const char      * email );
    Client & getClient(const char * p_nn);
    bool find(uint64_t & p_Index,const char * p_Name);

private:
    c_vector<Client> g_Clients;
};

CMailServer::CMailServer(const CMailServer &src) {}

CMailServer::CMailServer() {
}

void CMailServer::SendMail(const CMail &m) {

    getClient(m.getG_From().data()).sendMail(m);
    getClient(m.getG_To().data()).getMail(m);
}

CMailServer& CMailServer::operator=(const CMailServer &src) {

}

Client& CMailServer::getClient(const char *p_nn) {
    if(!g_Clients.size()){
        g_Clients.push_back(Client(p_nn));
        return g_Clients[0];
    }
    else if(strcmp(g_Clients[g_Clients.size()-1].getUsername(),p_nn)<0){
        g_Clients.push_back(Client(p_nn));
        return g_Clients[g_Clients.size()-1];
    }
    else if(strcmp(p_nn,g_Clients[0].getUsername())<0) {
        g_Clients.insert(0, Client(p_nn));
        return g_Clients[0];
    }else{
        uint64_t p_Position;
        if(find(p_Position,p_nn))
            return g_Clients[p_Position];
        else{
            g_Clients.insert(p_Position,Client(p_nn));
            return g_Clients[p_Position];
        };
    }
}

bool CMailServer::find(uint64_t &p_Index,const char * p_Name) {
    uint64_t l_left = 0,l_right = g_Clients.size();
    while(l_left<=l_right){
        p_Index = (l_left+l_right)/2;
        if(!strcmp(g_Clients[p_Index].getUsername(),p_Name)){
            return true;
        }
        else if(strcmp(g_Clients[p_Index].getUsername(),p_Name)<0)
            l_left=p_Index+1;
        else
            l_right=p_Index-1;
    }

    return false;
}

CMailIterator CMailServer::Inbox(const char *email)  {
    return getClient(email).openInbox();
}

CMailIterator CMailServer::Outbox(const char *email) {
    return getClient(email).openOutbox();
}
//------------------------------------------------END-CMAIL-SERVER------------------------------------------------------

//-----------------------------------------------------mine-------------------------------------------------------------

#ifndef __PROGTEST__
int main ( void )
{
    char from[100], to[100], body[1024];

    int * p = new int[10];


    assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
    assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
    CMailServer s0;
    s0 . SendMail ( CMail ( "john", "peter", "some important mail" ) );
    strncpy ( from, "john", sizeof ( from ) );
    strncpy ( to, "thomas", sizeof ( to ) );
    strncpy ( body, "another important mail", sizeof ( body ) );
    s0 . SendMail ( CMail ( from, to, body ) );
    strncpy ( from, "john", sizeof ( from ) );
    strncpy ( to, "alice", sizeof ( to ) );
    strncpy ( body, "deadline notice", sizeof ( body ) );
    s0 . SendMail ( CMail ( from, to, body ) );
    s0 . SendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
    s0 . SendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
    CMailIterator i0 = s0 . Inbox ( "alice" );
    assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( ! ++i0 );

    CMailIterator i1 = s0 . Inbox ( "john" );
    assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
    assert ( ! ++i1 );

    CMailIterator i2 = s0 . Outbox ( "john" );
    assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
    assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
    assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( ! ++i2 );

    CMailIterator i3 = s0 . Outbox ( "thomas" );
    assert ( ! i3 );

    CMailIterator i4 = s0 . Outbox ( "steve" );
    assert ( ! i4 );

    CMailIterator i5 = s0 . Outbox ( "thomas" );
    s0 . SendMail ( CMail ( "thomas", "boss", "daily report" ) );
    assert ( ! i5 );

    CMailIterator i6 = s0 . Outbox ( "thomas" );
    assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
    assert ( ! ++i6 );

    CMailIterator i7 = s0 . Inbox ( "alice" );
    s0 . SendMail ( CMail ( "thomas", "alice", "meeting details" ) );
    assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( ! ++i7 );

    CMailIterator i8 = s0 . Inbox ( "alice" );
    assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( ! ++i8 );


    //-------------------------------------------------------------------

    CMailServer s1 ( s0 );
    s0 . SendMail ( CMail ( "joe", "alice", "delivery details" ) );
    s1 . SendMail ( CMail ( "sam", "alice", "order confirmation" ) );
    CMailIterator i9 = s0 . Inbox ( "alice" );
    assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( ! ++i9 );

    CMailIterator i10 = s1 . Inbox ( "alice" );
    assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
    assert ( ! ++i10 );

    CMailServer s2;
    s2 . SendMail ( CMail ( "alice", "alice", "mailbox test" ) );
    CMailIterator i11 = s2 . Inbox ( "alice" );
    assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
    assert ( ! ++i11 );

    s2 = s0;
    s0 . SendMail ( CMail ( "steve", "alice", "newsletter" ) );
    s2 . SendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
    CMailIterator i12 = s0 . Inbox ( "alice" );
    assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
    assert ( ! ++i12 );

    CMailIterator i13 = s2 . Inbox ( "alice" );
    assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
    assert ( ! ++i13 );

    return 0;
}
#endif /* __PROGTEST__ */
