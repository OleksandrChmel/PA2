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

class String {
private:
    char *g_Data = nullptr;
public:
/*Copies one char field to another
 *
 * @param other - pointer on text field i want to copy
 * */
    String(const char *p_other) {
        if (p_other == nullptr)
            return;
        g_Data = new char[strlen(p_other) + 1];
        strcpy(g_Data, p_other);
    }

    /*
     * Almost the same logic as in method below. Firstly clean memory before copying
     * */

    String &operator=(const String &other) {
        if (!g_Data)
            delete[] g_Data;
        g_Data = nullptr;
        g_Data = new char[strlen(other.data()) + 1];
        strcpy(g_Data, other.data());

        return *this;
    }

    // Compare to strings
    //return 0 if strings are the same

    int strcmp(const char *p_other) const {
        return std::strcmp(g_Data, p_other);
    }

    const char *data() const {
        return g_Data;
    }

    void erase() {
        delete[] g_Data;
        g_Data = nullptr;
    }
};

//------------------------------------------------CMAIL-STRING-END------------------------------------------------------


//-----------------------------------------------------CMAIL------------------------------------------------------------
class CMail {
public:

    CMail(const char *from,
          const char *to,
          const char *body);

    ~CMail() {
        clean();
    }

    bool operator==(const CMail &x) const;

    const String &getG_From() const;

    const String &getG_To() const;

    const String &getG_Body() const;

    //Clean memory on delete

    void clean() {
        g_From.erase();
        g_To.erase();
        g_Body.erase();
    }

private:
    String g_From;
    String g_To;
    String g_Body;
};

CMail::CMail(const char *from, const char *to, const char *body) : g_From(from), g_To(to), g_Body(body) {
}

//Compare two emails and return true if they are the same, in opposite  situation false

bool CMail::operator==(const CMail &x) const {
    return !strcmp(g_From.data(), x.getG_From().data()) && !g_To.strcmp(x.getG_To().data()) &&
           !g_Body.strcmp(x.getG_Body().data());
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

//------------------------------------------------CMAIL-END-------------------------------------------------------------
//-------------------------------------------------CONTAINER------------------------------------------------------------

/*Simple detail of CmailList
 * Store one CMAIL and pointer to the next Container
 * */

class Container {
public:
    CMail g_mail = CMail(nullptr, nullptr, nullptr);
    Container *next;

    Container(const CMail &mail) : next(NULL) {
        g_mail = CMail(mail.getG_From().data(), mail.getG_To().data(), mail.getG_Body().data());
    };

};

//-----------------------------------------------END-CONTAINER----------------------------------------------------------

//------------------------------------------------CMAIL-LIST------------------------------------------------------------

/*Represent a one-way list, where you have access to the begin and the end of list
 * */

class CMailList {
public:

    CMailList() : g_begin(nullptr), g_last(nullptr) {}

    /*Add new Container after last inserted Container and move pointer g_last on the next element
     *
     * @param p_Container - element that will be inserted as last in list
     */

    void add(const CMail &p_Container) {

        if (!g_last)
            g_begin = g_last = new Container(p_Container);
        else {
            g_last->next = new Container(p_Container);
            g_last = g_last->next;
        }
    }

    /*
     *Copy all inside elements of _other CMailList to this CMailList, not references on object
     *
     * @param other - another CMailList
     *
     * @return reference on this
     * */

    CMailList &deepcopy(const CMailList &other) {
        Container *tmp = other.begin();
        while (tmp) {
            add(tmp->g_mail);
            tmp = tmp->next;
        }

        return *this;
    }

    //free memory on list delete

    void erase() {
        if (g_begin == nullptr)
            return;

        while (g_begin) {
            Container *tmp = g_begin->next;
            g_begin->g_mail.clean();
            delete g_begin;
            g_begin = tmp;
        }
    }

    Container *begin() const {
        return g_begin;
    }

    Container *end() {
        return g_last;
    }


private:
    Container *g_begin;
    Container *g_last;
};

//------------------------------------------------CMAIL-LIST-END--------------------------------------------------------

//------------------------------------------------CMAIL-ITERATOR--------------------------------------------------------

class CMailIterator {
public:
    CMailIterator(const CMailList &other) {
        list.deepcopy(other);
        g_begin = list.begin();
        g_end = list.end();
    }

    //return true if pointer on object is not null

    operator bool(void) const {
        if (!g_end || g_end->next == g_begin)
            return false;
        return true;
    }

    ~CMailIterator() {
        list.erase();
    }

    //The same logic as operator bool but return nagative result

    bool operator!(void) const {
        if (!g_begin || g_begin == g_end->next)
            return true;
        else return false;
    }

    const CMail &operator*(void) const {
        return g_begin->g_mail;
    }

    //Move pointer in list on next email

    CMailIterator &operator++(void) {
        g_begin = g_begin->next;
        return *this;
    }

private:
    CMailList list;
    Container *g_begin = nullptr;
    Container *g_end = nullptr;
};

//-----------------------------------------------END-CMAIL-ITERATOR-----------------------------------------------------

// ---------------------------------------------------CLIENT------------------------------------------------------------

class Client {
public:
    Client() : Inbox(), Outbox() {};

    Client &operator=(const Client &other) {
        Inbox = other.getInbox();
        Outbox = other.getOutbox();

        return *this;
    }

    /*
     * Copy Inbox and Outbox of one Client to another, not references on object
     *
     * @param other - another Client
     *
     * @return reference on this
     * */

    Client &deepcopy(const Client &other) {
        Inbox.deepcopy(other.getInbox());
        Outbox.deepcopy(other.getOutbox());

        return *this;
    }

    Client(const char *username) : Inbox(), Outbox() {}

    ~Client() {
    }

    /*Gets username of Client
     *
     * @return pointer to contant char field that contains email of this Client
     *
     * */

    const char *getUsername() const {
        if (Inbox.begin())
            return Inbox.begin()->g_mail.getG_To().data();
        else return Outbox.begin()->g_mail.getG_From().data();
    }

    void getMail(const CMail &p_email) {
        Inbox.add(p_email);
    }

    void sendMail(const CMail &p_email) {
        Outbox.add(p_email);
    }

    CMailIterator openInbox() {
        return CMailIterator(getInbox());
    }

    CMailIterator openOutbox() {
        return CMailIterator(getOutbox());
    }

    CMailList getInbox() const { return Inbox; }

    CMailList getOutbox() const { return Outbox; }

private:
    CMailList Inbox;
    CMailList Outbox;
};

//---------------------------------------------------END-CLIENT---------------------------------------------------------


//--------------------------------------------------COMPLEX-VECTOR------------------------------------------------------

/*Clever data storage that compactly use PC memory
 * */

class CleverVector {
private:
    struct Data {
        Client *m_Data;
        size_t m_Size;
        size_t m_LastIndex;
        size_t m_RefCnt;

        Client operator[](size_t idx) const;

        Client &operator[](size_t idx);

        size_t size() const { return m_LastIndex; }

        Data(size_t size);

        Data(const Data &other);

        ~Data();

        Data &operator=(const Data &other);

        void push_back(const Client &p_Client);

        void insert(uint64_t p_Position, const Client &p_Data);

        void realloc();
    };

    Data *m_Ptr;

    void detach();

    void attach(Data *ptr);

public:
    CleverVector();

    CleverVector(const CleverVector &other);

    ~CleverVector();

    CleverVector &operator=(const CleverVector &other);

    size_t getSize() const;

    Client operator[](size_t idx) const;

    Client &operator[](size_t idx);

    Data *getData() { return m_Ptr; }

    class DimensionException {
    };

};

CleverVector::CleverVector() {
    m_Ptr = new Data(1);
}

CleverVector::CleverVector(const CleverVector &other) {
    attach(other.m_Ptr);
}

CleverVector &CleverVector::operator=(const CleverVector &other) {
    if (this == &other) return *this;

    detach();
    attach(other.m_Ptr);
    return *this;
}

CleverVector::~CleverVector() {
    detach();
}

/*
 * Delete previous access to allocated memory and delete leaks if thay are not used by any other CleverVector
 *
 * works with this
 * */

void CleverVector::detach() {
    if (--m_Ptr->m_RefCnt == 0)
        delete m_Ptr;
    m_Ptr = nullptr;
}

/*Get access to allocated before memory
 *
 * @param ptr - pointer to data field
 * */

void CleverVector::attach(Data *ptr) {
    m_Ptr = ptr;
    m_Ptr->m_RefCnt += 1;
}


size_t CleverVector::getSize() const { return m_Ptr->size(); }

Client CleverVector::operator[](size_t idx) const { return (*m_Ptr)[idx]; }

/*Allows access to cell in memory on specific position
 *
 * @param idx - postion from where get data
 *
 * @return reference on Client
 * */

Client &CleverVector::operator[](size_t idx) {
    if (m_Ptr->m_RefCnt > 1) {
        Data *tmp = new Data(*m_Ptr);
        detach();
        attach(tmp);
    }

    return (*m_Ptr)[idx];
}

//---------------------------------------------------COMPLEX-VECTOR::DATA-----------------------------------------------
/*Almost the same design as in stl::vector
 * Represents data as chain
 * */

/*Add data after last-inserted element
 *
 * @param p_Client - a constant reference to Client
 * */

void CleverVector::Data::push_back(const Client &p_Client) {
    if (m_Size == m_LastIndex) {
        realloc();
    }

    m_Data[m_LastIndex++] = p_Client;
}

/*Insert element on specific position
 *
 * @param p_Position - position to insert
 * @param p_Data - a constant reference to Client
 * */

void CleverVector::Data::insert(uint64_t p_Position, const Client &p_Data) {
    Client tmp = p_Data;
    while (p_Position < m_LastIndex) {
        Client l_tmp = m_Data[p_Position];
        m_Data[p_Position] = tmp;
        tmp = l_tmp;
        p_Position++;
    }
    push_back(tmp);
}

/*Alllocate memory for data in vector
 * */

void CleverVector::Data::realloc() {

    m_Size *= 2;
    Client *tmp = new Client[m_Size];
    for (size_t i = 0; i < m_LastIndex; ++i) {
        tmp[i] = m_Data[i];
    }
    delete[] m_Data;
    m_Data = tmp;
}


/*Standart constructor
 * allocate memory for data where called
 * */

CleverVector::Data::Data(size_t size) {
    m_Data = new Client[size];
    m_Size = size;
    m_LastIndex = 0;
    m_RefCnt = 1;
}

/*Standart destructor
 * free memory
 */

CleverVector::Data::~Data() {
    for (size_t i = 0; i < m_LastIndex; ++i) {
        m_Data[i].getInbox().erase();
        m_Data[i].getOutbox().erase();
    }

    delete[] m_Data;
}

/*Copies data from one CleverVector to another
 *
 * @param other - reference to another CleverVector
 *
 * @return reference on this
 * */

CleverVector::Data::Data(const Data &other) {
    m_Data = new Client[other.m_Size];

    m_Size = other.m_Size;
    m_LastIndex = other.m_LastIndex;

    //copies each data cell

    for (size_t i = 0; i < m_LastIndex; ++i)
        m_Data[i].deepcopy(other.m_Data[i]);

    m_RefCnt = 0;
}

CleverVector::Data &CleverVector::Data::operator=(const CleverVector::Data &other) {
    if (this == &other) return *this;

    delete[] m_Data;

    m_Data = new Client[other.m_Size];

    m_Size = other.m_Size;
    m_LastIndex = other.m_LastIndex;


    //copies each data cell

    for (size_t i = 0; i < m_LastIndex; ++i)
        m_Data[i] = other.m_Data[i];

    return *this;
}

/*Allows access to cell in memory on specific position
 *
 * @param idx - postion from where get data
 *
 * @return class Client if i want to change object or reference if don't want to change object
 *
 * @throw DimensionException() if got index don't exist
 *
 * */

Client CleverVector::Data::operator[](size_t idx) const {
    if (idx >= m_LastIndex)
        throw DimensionException();
    return m_Data[idx];
}

Client &CleverVector::Data::operator[](size_t idx) {
    if (idx >= m_LastIndex)
        throw DimensionException();
    return m_Data[idx];
}
//-----------------------------------------------COMPLEX-VECTOR-END-----------------------------------------------------

//--------------------------------------------------CMAIL-SERVER--------------------------------------------------------
class CMailServer {
public:
    CMailServer(void) {}

    CMailServer(const CMailServer &src);

    CMailServer &operator=(const CMailServer &src);

    ~CMailServer(void) {}

    void SendMail(const CMail &m);

    CMailIterator Outbox(const char *email) const;

    CMailIterator Inbox(const char *email) const;

    Client &getClient(const char *p_nn);

    Client getClient(const char *p_nn) const;

    const CleverVector &getData() const { return g_Clients; }

    bool find(uint64_t &p_Index, const char *p_Name) const;

private:
    // Clever vector that effectively use memory
    CleverVector g_Clients;
};

CMailServer::CMailServer(const CMailServer &src) {
    g_Clients = src.getData();
}

void CMailServer::SendMail(const CMail &m) {

    getClient(m.getG_From().data()).sendMail(m);
    getClient(m.getG_To().data()).getMail(m);

}

CMailServer &CMailServer::operator=(const CMailServer &src) {
    g_Clients = src.getData();

    return *this;
}

/*Return a reference to Client from vector
 * If Client is not in field insert it on specific position
 *
 * @param p_nn - Client's username
 * @return reference to Client
 * */

Client &CMailServer::getClient(const char *p_nn) {
    if (!g_Clients.getSize()) {
        (g_Clients.getData())->push_back(Client(p_nn));
        return g_Clients[0];
    } else if (strcmp(g_Clients[g_Clients.getSize() - 1].getUsername(), p_nn) < 0) {
        g_Clients.getData()->push_back(Client(p_nn));
        return g_Clients[g_Clients.getSize() - 1];
    } else if (strcmp(p_nn, g_Clients[0].getUsername()) < 0) {
        g_Clients.getData()->insert(0, Client(p_nn));
        return g_Clients[0];
    } else {
        uint64_t p_Position = 0;
        if (find(p_Position, p_nn))
            return g_Clients[p_Position];
        else {
            uint64_t  i = 0;
            while(strcmp(p_nn,g_Clients[i].getUsername())>0)
                i++;
            g_Clients.getData()->insert(i, Client(p_nn));
            return g_Clients[i];
        };
    }
}

/*If Client dont't exist in field return empty Client
 * */

Client CMailServer::getClient(const char *p_nn) const {
    uint64_t l_Index = 0;
    if (find(l_Index, p_nn))
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

bool CMailServer::find(uint64_t &p_Index, const char *p_Name) const {
    int l_left = 0, l_right = g_Clients.getSize()-1;
    while (l_left <= l_right) {
        p_Index = (l_left + l_right) / 2;
        int res = strcmp(g_Clients[p_Index].getUsername(), p_Name);
        if (!res) {
            return true;
        } else if (res < 0)
            l_left = p_Index + 1;
        else
            l_right = p_Index - 1;
    }

    return false;
}

/*These two methods allows to work with Inbox and Outbox
 *
 * @param email - email of client
 *
 * @return CMailIterator - a class that allow look through the list of emails
 * */

CMailIterator CMailServer::Inbox(const char *email) const {
    uint64_t l_Index = 0;
    if (find(l_Index, email))
        return g_Clients[l_Index].openInbox();
    return CMailIterator(CMailList());
}

CMailIterator CMailServer::Outbox(const char *email) const {
    uint64_t l_Index = 0;
    if (find(l_Index, email))
        return g_Clients[l_Index].openOutbox();
    return CMailIterator(CMailList());
}
//------------------------------------------------END-CMAIL-SERVER------------------------------------------------------



//-----------------------------------------------------mine-------------------------------------------------------------

#ifndef __PROGTEST__

int main(void) {
    char from[100], to[100], body[1024];


    assert (CMail("john", "peter", "progtest deadline") == CMail("john", "peter", "progtest deadline"));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("john", "progtest deadline", "peter")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("peter", "john", "progtest deadline")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("peter", "progtest deadline", "john")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "john", "peter")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "peter", "john")));

    CMailServer d;
    d.SendMail(CMail("as", "av", "dfdfb"));

    CMailServer d1 = d;
    d1.SendMail(CMail("as", "av", "asdasd"));

    CMailServer d2(d1);
    d2.SendMail(CMail("as", "av", "bbbbb"));

    CMailIterator ii = d1.Inbox("av");
    assert(*ii == CMail("as", "av", "dfdfb"));
    assert(*(++ii) == CMail("as", "av", "asdasd"));
    assert(!++ii);

    CMailIterator ii2 = d1.Inbox("as");
    assert(!ii2);
    CMailIterator ii3 = d1.Inbox("adfsa");
    assert(!ii3);

    CMailServer s;
    s.SendMail(CMail("aa", "asdfg", ""));
    s.SendMail(CMail("aa", "sf", ""));
    s.SendMail(CMail("ab", "sfd", ""));
    s.SendMail(CMail("ac", "gs", ""));
    s.SendMail(CMail("ad", "dgdsa", ""));
    s.SendMail(CMail("ag", "dsg", ""));
    s.SendMail(CMail("ah", "hsfa", ""));
    s.SendMail(CMail("ar", "gsa", ""));
    s.SendMail(CMail("ah", "hfdsg", ""));
    s.SendMail(CMail("af", "asg", ""));
    s.SendMail(CMail("ah", "adg", ""));
    s.SendMail(CMail("af", "sagf", ""));
    s.SendMail(CMail("fd", "sfdf", ""));
    s.SendMail(CMail("dgg", "fdgd", ""));
    s.SendMail(CMail("dg", "safg", ""));
    s.SendMail(CMail("dg", "afd", ""));
    s.SendMail(CMail("dgdf", "ff", ""));
    s.SendMail(CMail("dg", "", "dfgf"));
    s.SendMail(CMail("fdg", "sagd", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("dgfd", "fdg", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("df", "dfg", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("dfg", "gaae", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("dgfg", "hhtt", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("dgd", "nh", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("dgdf", "hnmhdgm", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("fgd", "dfhnhg", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("dfgd", "dhn", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("dfd", "hg", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("ege", "gbngh", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("ertw", "bgdf", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("wrtrw", "fdn", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("tert", "bddddddddddddddd", "rgth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("rtet", "bny", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("tree", "hdfhn", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("uyrrt", "ndhgm", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("agree", "hdhn", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("et", "h", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("trht", "j", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("tret", "hy", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("ethr", "t", "rth bberhtejjejdfhcdjsfjs"));
    s.SendMail(CMail("trrth", "e", "rth bberhtejjejdfhcdjsfjs"));

    CMailServer s10 = s;
    CMailServer s20 = s;
    CMailServer s30 = s;
    CMailServer s40 = s;
    CMailServer s50 = s;
    CMailServer s60 = s;

    s20.SendMail(CMail("sdsd", "fg", "g"));

    CMailServer s0;
    s0.SendMail(CMail("john", "peter", "some important mail"));
    strncpy(from, "john", sizeof(from));
    strncpy(to, "thomas", sizeof(to));
    strncpy(body, "another important mail", sizeof(body));
    s0.SendMail(CMail(from, to, body));
    strncpy(from, "john", sizeof(from));
    strncpy(to, "alice", sizeof(to));
    strncpy(body, "deadline notice", sizeof(body));
    s0.SendMail(CMail(from, to, body));
    s0.SendMail(CMail("alice", "john", "deadline confirmation"));
    s0.SendMail(CMail("peter", "alice", "PR bullshit"));
    CMailIterator i0 = s0.Inbox("alice");
    assert (i0 && *i0 == CMail("john", "alice", "deadline notice"));
    assert (++i0 && *i0 == CMail("peter", "alice", "PR bullshit"));
    assert (!++i0);

    CMailIterator i1 = s0.Inbox("john");
    assert (i1 && *i1 == CMail("alice", "john", "deadline confirmation"));
    assert (!++i1);

    CMailIterator i2 = s0.Outbox("john");
    assert (i2 && *i2 == CMail("john", "peter", "some important mail"));
    assert (++i2 && *i2 == CMail("john", "thomas", "another important mail"));
    assert (++i2 && *i2 == CMail("john", "alice", "deadline notice"));
    assert (!++i2);

    CMailIterator i3 = s0.Outbox("thomas");
    assert (!i3);

    CMailIterator i4 = s0.Outbox("steve");
    assert (!i4);

    CMailIterator i5 = s0.Outbox("thomas");
    s0.SendMail(CMail("thomas", "boss", "daily report"));
    assert (!i5);

    CMailIterator i6 = s0.Outbox("thomas");
    assert (i6 && *i6 == CMail("thomas", "boss", "daily report"));
    assert (!++i6);

    CMailIterator i7 = s0.Inbox("alice");
    s0.SendMail(CMail("thomas", "alice", "meeting details"));
    assert (i7 && *i7 == CMail("john", "alice", "deadline notice"));
    assert (++i7 && *i7 == CMail("peter", "alice", "PR bullshit"));
    assert (!++i7);

    CMailIterator i8 = s0.Inbox("alice");
    assert (i8 && *i8 == CMail("john", "alice", "deadline notice"));
    assert (++i8 && *i8 == CMail("peter", "alice", "PR bullshit"));
    assert (++i8 && *i8 == CMail("thomas", "alice", "meeting details"));
    assert (!++i8);


    //-------------------------------------------------------------------

    CMailServer s1(s0);
    s0.SendMail(CMail("joe", "alice", "delivery details"));
    s1.SendMail(CMail("sam", "alice", "order confirmation"));
    CMailIterator i9 = s0.Inbox("alice");
    assert (i9 && *i9 == CMail("john", "alice", "deadline notice"));
    assert (++i9 && *i9 == CMail("peter", "alice", "PR bullshit"));
    assert (++i9 && *i9 == CMail("thomas", "alice", "meeting details"));
    assert (++i9 && *i9 == CMail("joe", "alice", "delivery details"));
    assert (!++i9);

    CMailIterator i10 = s1.Inbox("alice");
    assert (i10 && *i10 == CMail("john", "alice", "deadline notice"));
    assert (++i10 && *i10 == CMail("peter", "alice", "PR bullshit"));
    assert (++i10 && *i10 == CMail("thomas", "alice", "meeting details"));
    assert (++i10 && *i10 == CMail("sam", "alice", "order confirmation"));
    assert (!++i10);

    CMailServer s2;
    s2.SendMail(CMail("alice", "alice", "mailbox test"));
    CMailIterator i11 = s2.Inbox("alice");
    assert (i11 && *i11 == CMail("alice", "alice", "mailbox test"));
    assert (!++i11);

    s2 = s0;
    s0.SendMail(CMail("steve", "alice", "newsletter"));
    s2.SendMail(CMail("paul", "alice", "invalid invoice"));
    CMailIterator i12 = s0.Inbox("alice");
    assert (i12 && *i12 == CMail("john", "alice", "deadline notice"));
    assert (++i12 && *i12 == CMail("peter", "alice", "PR bullshit"));
    assert (++i12 && *i12 == CMail("thomas", "alice", "meeting details"));
    assert (++i12 && *i12 == CMail("joe", "alice", "delivery details"));
    assert (++i12 && *i12 == CMail("steve", "alice", "newsletter"));
    assert (!++i12);

    CMailIterator i13 = s2.Inbox("alice");
    assert (i13 && *i13 == CMail("john", "alice", "deadline notice"));
    assert (++i13 && *i13 == CMail("peter", "alice", "PR bullshit"));
    assert (++i13 && *i13 == CMail("thomas", "alice", "meeting details"));
    assert (++i13 && *i13 == CMail("joe", "alice", "delivery details"));
    assert (++i13 && *i13 == CMail("paul", "alice", "invalid invoice"));
    assert (!++i13);

    CMailServer *test;
    test = new CMailServer();
    test->SendMail(CMail("haranti", "elisa", "Lol plesatec xD tahni"));
    test->SendMail(CMail("elisa", "buh", "oni se mi smeji :("));
    test->SendMail(CMail("buh", "medvedice", "plz kill, alespon 42 z nich"));
    test->SendMail(CMail("buh", "elisa", "vyrizeno xD"));

    // ulozit
    CMailIterator elisa_odeslane = test->Outbox("elisa");
    CMailIterator elisa_prijate = test->Inbox("elisa");

    // a znicit
    delete test;

    assert(elisa_odeslane && *elisa_odeslane == CMail("elisa", "buh", "oni se mi smeji :("));
    assert(!++elisa_odeslane);

    assert(elisa_prijate && *elisa_prijate == CMail("haranti", "elisa", "Lol plesatec xD tahni"));
    assert(++elisa_prijate && *elisa_prijate == CMail("buh", "elisa", "vyrizeno xD"));
    assert(!++elisa_prijate);

    return 0;
}

#endif /* __PROGTEST__ */
