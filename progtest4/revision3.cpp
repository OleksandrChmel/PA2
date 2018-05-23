#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

//---------------------------------------------------mine---------------------------------------------------------------

// class STRING
// implements functions for working with char chains

class String{
private:
    char * g_Data = nullptr;
public:

    String(const char * p_other){
        if(p_other== nullptr)
            return ;
        g_Data = new char[strlen(p_other)+1];
        strcpy(g_Data,p_other);
    }
    String &operator = (const String & other){
        if(!g_Data)
            delete [] g_Data;
        g_Data = new char[strlen(other.data())+1];
        strcpy(g_Data,other.data());

        return *this;
    }
    int strcmp(const char * p_other)const{
        return std::strcmp(g_Data,p_other);
    }
    const char * data()const {
        return g_Data;
    }
    void erase(){
        delete [] g_Data;
        g_Data = nullptr;
    }
};

//------------------------------------------------CMAIL-STRING-END------------------------------------------------------

//-----------------------------------------------------CMAIL------------------------------------------------------------
class CMail
{
public:

    CMail                         ( const char      * from,
                                    const char      * to,
                                    const char      * body );
    ~CMail(){
            clean();
    }
    bool                     operator ==                   ( const CMail     & x ) const;

    const String &getG_From() const;

    const String &getG_To() const;

    const String &getG_Body() const;

    void clean(){
        g_From.erase();
        g_To.erase();
        g_Body.erase();
    }
private:
    String g_From;
    String g_To;
    String g_Body;
};

CMail::CMail(const char *from, const char *to, const char *body):g_From(from),g_To(to),g_Body(body) {
}

bool CMail::operator==(const CMail &x) const {
    if(!g_From.strcmp(x.getG_From().data()) && !g_To.strcmp(x.getG_To().data()) && !g_Body.strcmp(x.getG_Body().data()))
        return true;
    else return false;
}


const String &CMail::getG_From() const {
    return g_From;
}

const String &CMail::getG_To() const {
    return g_To;
}

const String &CMail::getG_Body() const {
    return g_Body;
}

class Container{
public:
    CMail g_mail = CMail(nullptr, nullptr, nullptr);
    Container * next;

    Container(const CMail & mail):next(NULL){
        g_mail = CMail(mail.getG_From().data(),mail.getG_To().data(),mail.getG_Body().data());
    };

};

//------------------------------------------------CMAIL-END-------------------------------------------------------------

//------------------------------------------------CMAIL-LIST------------------------------------------------------------

class CMailList{
public:

    CMailList():g_begin(nullptr),g_last(nullptr){}
    void add(const CMail & p_Container){

        if(!g_last)
            g_begin = g_last = new Container(p_Container);
        else {
            g_last->next = new Container(p_Container);
            g_last = g_last->next;
        }
    }
    CMailList & deepcopy(const CMailList & other){
        Container * tmp = other.begin();
        while(tmp){
            add(tmp->g_mail);
            tmp = tmp->next;
        }

        return  *this;
    }
    void erase(){
        if(g_begin== nullptr)
            return;

        while(g_begin){
            Container * tmp = g_begin->next;
            g_begin->g_mail.clean();
            delete g_begin;
            g_begin = tmp;
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

//------------------------------------------------CMAIL-LIST-END--------------------------------------------------------

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
    Client & operator = (const Client & other){
        Inbox = other.getInbox();
        Outbox = other.getOutbox();

        return *this;
    }
    Client & deepcopy(const Client & other){
        Inbox.deepcopy(other.getInbox());
        Outbox.deepcopy(other.getOutbox());

        return *this;
    }
    Client(const char * username):Inbox(),Outbox(){}
    ~Client(){
    }
    const char * getUsername()const {
        if(Inbox.begin()!= nullptr )
            return Inbox.begin()->g_mail.getG_To().data();
        else return Outbox.begin()->g_mail.getG_From().data();
    }
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
    CMailList getInbox()const{ return Inbox; }
    CMailList getOutbox()const{ return Outbox; }
private:
    //String g_username;
    CMailList Inbox;
    CMailList Outbox;
};

//---------------------------------------------------END-CLIENT---------------------------------------------------------


//--------------------------------------------------COMPLEX-VECTOR------------------------------------------------------

/*Clever data storage that compactly use PC memory
 * */

class ComplexVector
{
private:
    struct Data
    {
        Client *m_Data;
        size_t   m_Size;
        size_t m_LastIndex;
        size_t   m_RefCnt;

        Client  operator [ ] ( size_t idx ) const;
        Client& operator [ ] ( size_t idx );
        size_t size ( ) const { return m_LastIndex; }
        Data ( size_t size );
        Data ( const Data & other );
        ~Data ( );
        Data& operator = ( const Data& other );

        void push_back(const Client & p_Client);
        void insert(uint64_t p_Position,const Client & p_Data);
        void realloc();
    };

    Data * m_Ptr;
    void detach ( );
    void attach ( Data * ptr );

public:
    ComplexVector ();
    ComplexVector ( const ComplexVector & other );
    ~ComplexVector ( );
    ComplexVector& operator = ( const ComplexVector& other );

    size_t getSize ( ) const;

    Client operator [ ] ( size_t idx ) const;
    Client& operator [ ] ( size_t idx );

    Data * getData(){ return  m_Ptr; }

    class DimensionException { };

};

ComplexVector::ComplexVector ( )
{
    m_Ptr = new Data ( 1 );
}

ComplexVector::ComplexVector ( const ComplexVector & other )
{
    attach ( other.m_Ptr );
}

ComplexVector& ComplexVector::operator = ( const ComplexVector& other )
{
    if ( this == & other ) return *this;

    detach ( );
    attach ( other.m_Ptr );
    return *this;
}

ComplexVector::~ComplexVector ( )
{
    detach ( );
}

void ComplexVector::detach ( )
{
    if ( -- m_Ptr -> m_RefCnt == 0 )
        delete m_Ptr;
    m_Ptr = nullptr;
}

void ComplexVector::attach ( Data * ptr )
{
    m_Ptr = ptr;
    m_Ptr -> m_RefCnt += 1;
}



size_t ComplexVector::getSize ( ) const { return m_Ptr -> size ( ); }
Client ComplexVector::operator [ ] ( size_t idx ) const { return ( *m_Ptr ) [ idx ]; }

Client& ComplexVector::operator [ ] ( size_t idx )
{
    if ( m_Ptr -> m_RefCnt > 1 )
    {
        Data * tmp = new Data ( * m_Ptr );
        detach ( );
        attach ( tmp );
    }

    return (*m_Ptr) [ idx ];
}

//---------------------------------------------------COMPLEX-VECTOR::DATA-----------------------------------------------
/*Almost the same design as in stl::vector
 * Represents data as chain
 * */

/*Add data after last-inserted element
 *
 * @param p_Client - a constant reference to Client
 * */
void ComplexVector::Data::push_back(const Client &p_Client) {
    if(m_Size==m_LastIndex)
    {
        realloc();
    }

    m_Data[m_LastIndex++] = p_Client;
}

/*Insert element on specific position
 *
 * @param p_Position - position to insert
 * @param p_Data - a constant reference to Client
 * */

void ComplexVector::Data::insert(uint64_t p_Position, const Client &p_Data) {
    Client tmp = p_Data;
    while(p_Position<m_LastIndex)
    {
        Client l_tmp  = m_Data[p_Position];
        m_Data[p_Position] = tmp;
        tmp = l_tmp;
        p_Position++;
    }
    push_back(tmp);
}

/*Alllocate memory for data in vector
 * */

void ComplexVector::Data::realloc() {

    m_Size*=2;
    Client * tmp = new Client[m_Size];
    for (size_t i = 0; i < m_LastIndex; ++i) {
        tmp[i] = m_Data[i];
    }
    delete [] m_Data;
    m_Data = tmp;
}


/*Standart constructor
 * allocate memory for data where called
 * */

ComplexVector::Data::Data ( size_t size ) {
    m_Data = new Client [ size ];
    m_Size = size;
    m_LastIndex=0;
    m_RefCnt = 1;
}

/*Standart destructor
 * free memory
 */

ComplexVector::Data::~Data ( ) {
    for (size_t i = 0; i < m_LastIndex; ++i) {
        m_Data[i].getInbox().erase();
        m_Data[i].getOutbox().erase();
    }

    delete [ ] m_Data;
}

ComplexVector::Data::Data ( const Data & other )
{
    m_Data = new Client [ other.m_Size ];

    m_Size = other.m_Size;
    m_LastIndex = other.m_LastIndex;
    for ( size_t i = 0; i < m_LastIndex ; ++ i )
        m_Data [ i ].deepcopy(other.m_Data[i]);

    m_RefCnt = 0;
}

ComplexVector::Data& ComplexVector::Data::operator = ( const ComplexVector::Data & other )
{
    if ( this == & other ) return *this;

    delete [ ] m_Data;

    m_Data = new Client [ other.m_Size ];

    m_Size = other.m_Size;
    for ( size_t i = 0; i < m_Size ; ++ i )
        m_Data [ i ] = other.m_Data [ i ];

    return *this;
}

Client ComplexVector::Data::operator [ ] ( size_t idx ) const
{
    if ( idx >= m_Size )
        throw DimensionException ( );
    return m_Data [ idx ];
}

Client& ComplexVector::Data::operator [ ] ( size_t idx )
{
    if ( idx >= m_Size )
        throw DimensionException ( );
    return m_Data [ idx ];
}
//-----------------------------------------------COMPLEX-VECTOR-END-----------------------------------------------------

//--------------------------------------------------CMAIL-SERVER--------------------------------------------------------
class CMailServer
{
public:
    CMailServer                   ( void ){}
    CMailServer                   ( const CMailServer & src );
    CMailServer            & operator =                    ( const CMailServer & src );
    ~CMailServer                  ( void ){}
    void                     SendMail                      ( const CMail     & m );
    CMailIterator            Outbox                        ( const char      * email )const;
    CMailIterator            Inbox                         ( const char      * email )const;
    Client & getClient(const char * p_nn);
    Client getClient(const char * p_nn) const;
    const ComplexVector & getData()const{ return g_Clients; }
    bool find(uint64_t & p_Index,const char * p_Name)const;

private:
    ComplexVector g_Clients;
};

CMailServer::CMailServer(const CMailServer &src) {
    g_Clients = src.getData();
}

void CMailServer::SendMail(const CMail &m) {

    getClient(m.getG_From().data()).sendMail(m);
    getClient(m.getG_To().data()).getMail(m);
}

CMailServer& CMailServer::operator=(const CMailServer &src) {
    g_Clients = src.getData();

    return *this;
}

/*Return a reference to Client from vector
 * If Client is not in field insert it on specific position
 *
 * @param p_nn - Client's username
 * @return reference to Client
 * */

Client& CMailServer::getClient(const char *p_nn) {
    if(!g_Clients.getSize()){
        (g_Clients.getData())->push_back(Client(p_nn));
        return g_Clients[0];
    }
    else if(strcmp(g_Clients[g_Clients.getSize()-1].getUsername(),p_nn)<0){
        g_Clients.getData()->push_back(Client(p_nn));
        return g_Clients[g_Clients.getSize()-1];
    }
    else if(strcmp(p_nn,g_Clients[0].getUsername())<0) {
        g_Clients.getData()->insert(0, Client(p_nn));
        return g_Clients[0];
    }else{
        uint64_t p_Position;
        if(find(p_Position,p_nn))
            return g_Clients[p_Position];
        else{
            g_Clients.getData()->insert(p_Position,Client(p_nn));
            return g_Clients[p_Position];
        };
    }
}

/*If Client dont't exist in field return empty Client
 * */

Client CMailServer::getClient(const char *p_nn) const {
    uint64_t l_Index=0;
    if(find(l_Index,p_nn))
        return g_Clients[l_Index];
    else return Client(p_nn);
}

/*Find Client by eemail in vector with complexity Log2
 *
 * @param p_Name - email of client
 * @param p_Index - position of Client
 *
 * @return true on success and false if not
 * */

bool CMailServer::find(uint64_t &p_Index,const char * p_Name)const {
    uint64_t l_left = 0,l_right = g_Clients.getSize();
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

CMailIterator CMailServer::Inbox(const char *email) const {
    return getClient(email).openInbox();
}

CMailIterator CMailServer::Outbox(const char *email)const {
    return getClient(email).openOutbox();
}
//------------------------------------------------END-CMAIL-SERVER------------------------------------------------------



//-----------------------------------------------------mine-------------------------------------------------------------

#ifndef __PROGTEST__
int main ( void )
{
    char from[100], to[100], body[1024];


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
