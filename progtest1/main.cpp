#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cctype>
#include <climits>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;

const uint16_t ENDIAN_LITTLE = 0x4949;
const uint16_t ENDIAN_BIG    = 0x4d4d;

#endif /* __PROGTEST__ */

class Pixel{

    private:
    int g_X;
    int g_Y;
        char * g_Data;
    public:
        Pixel(int p_X,int p_Y, char * p_Data){
            g_X=p_X;
            g_Y=p_Y;
            g_Data = p_Data;
        }

        ~Pixel(){

        }

        int getX(){
            return g_X;
        }

        int getY(){
            return g_Y;
        }

        char * getDataByIndex(int p_I){
            return g_Data ? (g_Data+p_I) : NULL;
        }

        void setBuffer(char * p_Data){
            g_Data=p_Data;
        }

    char * getBuffer()const{
            return g_Data;
        }
};

class Header{

    private:
        bool g_LEnd;
        bool g_WEnd;
        uint16_t g_ImgWidth;
        uint16_t g_ImgHeight;
        uint8_t g_ChannelPixel;
        uint8_t g_BitChannel;
        uint8_t g_IInterleave;
        uint8_t g_Interleave;
    public:

        Header(){}

        bool setEndian(const char p_Part1 , const char p_Part2){
            uint16_t l_Endian = 0x0000;
            convertFrom8To16(l_Endian,(uint8_t)p_Part1 , (uint8_t)p_Part2);

            if(l_Endian==ENDIAN_LITTLE){
                g_LEnd=true;
                return true;}
            else if(l_Endian==ENDIAN_BIG){
                g_LEnd=false;
                return true;
            }
            else return false;
        }

        bool setWEnd(uint16_t l_Endian){
            if(l_Endian==ENDIAN_LITTLE){
                g_WEnd=true;
                return true;}
            else if(l_Endian==ENDIAN_BIG){
                g_WEnd=false;
                return true;
            }
            else return false;
        }

        bool getEndian(){
            return g_LEnd;
        }

        bool getWEnd(){
            return g_WEnd;
        }

        void setWidth(const char p_Part1 , const char p_Part2){
            g_ImgWidth=0;
            if(g_LEnd)
                convertFrom8To16(g_ImgWidth,(uint8_t)p_Part1,(uint8_t)p_Part2);
            else convertFrom8To16(g_ImgWidth,(uint8_t)p_Part2,(uint8_t)p_Part1);
        }

        void setHeight(const char p_Part1 , const char p_Part2){
            g_ImgHeight=0;
            if(g_LEnd)
                convertFrom8To16(g_ImgHeight,(uint8_t)p_Part1,(uint8_t)p_Part2);
            else convertFrom8To16(g_ImgHeight,(uint8_t)p_Part2,(uint8_t)p_Part1);
        }

        int getWidth()const{
            return g_ImgWidth;
        }

        int getHeight()const{
            return g_ImgHeight;
        }

        bool setChannelPixel(uint8_t p_Byte){
            g_ChannelPixel = 0;
            g_ChannelPixel =  (p_Byte << 6);
            g_ChannelPixel = g_ChannelPixel >> 6;

            if(g_ChannelPixel==0x00) g_ChannelPixel=1;
            else if(g_ChannelPixel==0x02) g_ChannelPixel=3;
            else if(g_ChannelPixel==0x03) g_ChannelPixel=4;
            else return false;

            return true;
        }

        int getChannelPixel()const{
            return (int)g_ChannelPixel;
        }

        uint8_t getBitChannelPixel(){
            if(g_ChannelPixel==1) return 0x00;
            else if(g_ChannelPixel==3) return 0x02;
            else return 0x03;
        }

        bool setBitChannel(uint8_t p_Byte){
            g_BitChannel=0;
            g_BitChannel = (p_Byte << 3);
            g_BitChannel = g_BitChannel >> 5;

            if(g_BitChannel==0x00 || g_BitChannel==0x03 || g_BitChannel==0x04){
                g_BitChannel=0x01<<g_BitChannel;
                return true;
            }
            else return false;
        }

        uint8_t getBitChannel(){
            return g_BitChannel;
        }

        bool setIInterleave(uint8_t p_Byte){
            if(p_Byte>6)
                return false;

            g_IInterleave=0x01<<p_Byte;
            return true;
        }

        uint8_t getIInterleave(){
            return g_IInterleave;
        }

        bool setInterleave(uint8_t p_Byte){
            g_Interleave = p_Byte;
            return true;
        }

        uint8_t getInterleave()const{
            return g_Interleave;
        }

        bool hasWrongHeader(const uint8_t p_Byte1,uint8_t p_Byte2,uint8_t p_Interleave){
            if(p_Byte1!=0x00 && g_LEnd)return true;
            else if(p_Byte2!=0x00 && !g_LEnd) return true;

            if(g_LEnd && (!setChannelPixel(p_Byte2) || !setBitChannel(p_Byte2) || !setInterleave(p_Interleave) || !setIInterleave(p_Byte2>>5))) return true;
            else if(!g_LEnd && (!setChannelPixel(p_Byte1) || !setBitChannel(p_Byte1) || !setInterleave(p_Interleave) || !setIInterleave(p_Byte1>>5))) return true;

            return false;
        }

        bool hasZeroSize(){
            if(g_ImgHeight * g_ImgWidth == 0) return true;
            else return false;
        }

        bool hasRightSize(size_t & p_FileLen){
            return (uint64_t)p_FileLen==(uint64_t)8+((uint64_t)g_ImgWidth*g_ImgHeight*g_ChannelPixel*g_BitChannel)/8;
        }

        void convertFrom8To16(uint16_t & p_Number,const uint8_t p_Part1 , const uint8_t p_Part2){
            p_Number = (uint16_t)p_Part1;
            p_Number = p_Number << 8;
            p_Number|=(uint16_t)p_Part2;
        }

        char convertFrom16To8(uint16_t p_Num,int p_Part){
            if(p_Part==2) {
                p_Num = p_Num >> 8;
                return (char) p_Num;
            }else{
                p_Num = p_Num << 8;
                p_Num = p_Num >> 8;
                return (char) p_Num;
            }
        }

        uint8_t getComplex(){
            uint8_t l_Byte = log2(g_Interleave);
            return (((l_Byte<<3)|(uint8_t)log2(g_BitChannel))<<2)|getBitChannelPixel();
        }


};

class Image{
    private:
        Header * g_Header;
        vector<Pixel> g_Data;
        char * g_Buffer;
        size_t g_FileLen;
    public:

        Image(){
            g_Buffer=NULL;
            g_FileLen=0;
        }

        ~Image(){
            if(g_Buffer)
                delete [] g_Buffer;
        }

        bool setImage(const char * p_Src,int & p_interleave,uint16_t p_ByteOrder){
            ifstream l_FileIn;
            l_FileIn.open(p_Src,ios::in|ios::binary);

            if(!l_FileIn || l_FileIn.fail()){
                l_FileIn.close();
                return false;
            }


            g_FileLen = getFileLenght(p_Src);

            g_Buffer = new char[8];

            if(!g_FileLen)return  false;
                l_FileIn.read(g_Buffer,8);
            if(!l_FileIn || l_FileIn.fail()) return false;

            if(!setHeader(p_interleave,p_ByteOrder))
                return false;

            if(!setPixels(l_FileIn))
                return false;

            l_FileIn.close();
            return true;
        }

    bool setHeader(int & p_Interleave,uint16_t p_BO){
        g_Header = new Header;

        if(!g_Header->setEndian(g_Buffer[1],g_Buffer[0]))
            return false;

        if(!g_Header->setWEnd(p_BO))
            return false;

        g_Header->setWidth(g_Buffer[3],g_Buffer[2]);
        g_Header->setHeight(g_Buffer[5],g_Buffer[4]);

        if(g_Header->hasZeroSize())
            return false;

        if(g_Header->hasWrongHeader((uint8_t)g_Buffer[7],(uint8_t)g_Buffer[6],p_Interleave))
            return false;

        if(!g_Header->hasRightSize(g_FileLen))
            return false;

        //if((int)g_Header->getInterleave()!=p_Interleave)
          //  return false;

        return true;
    }

    bool setPixels(ifstream & p_File) {
        int l_W=g_Header->getWidth();
        int l_H=g_Header->getHeight();
        int l_CP = g_Header->getChannelPixel()*g_Header->getBitChannel()/8;

        for (int i = 0; i < l_H; ++i) {
            for (int j = 0; j < l_W; ++j) {
                char * buff = new char[l_CP];
                p_File.read(buff,l_CP);
                if(!p_File || p_File.fail()) return false;

                g_Data.push_back(Pixel(i,j,buff));
            }
        }

        delete [] g_Buffer;
        g_Buffer=NULL;

        return true;
    }

    bool writeToFile(const char * dstFile){
        ofstream l_File(dstFile);

        if(!l_File || l_File.fail())
            return false;

        if(g_Header->getWEnd()) {
            char * b = new char[8];
            b[0] =  (char) 0x49;
            b[1] = (char) 0x49;
            b[2] = g_Header->convertFrom16To8(g_Header->getWidth(), 1);
            b[3] = g_Header->convertFrom16To8(g_Header->getWidth(), 2);
            b[4] = g_Header->convertFrom16To8(g_Header->getHeight(), 1);
            b[5] = g_Header->convertFrom16To8(g_Header->getHeight(), 2);
            b[6] = (char) g_Header->getComplex();
            b[7] = (char) 0x00;
            l_File.write(b,8);
            if(!l_File || l_File.fail()) return false;
        }
        else if(!g_Header->getWEnd() && l_File.is_open() && !l_File.fail()) {
            char * b = new char[8];
            b[0] = (char) 0x4d;
            b[1] = (char) 0x4d;
            b[2] = g_Header->convertFrom16To8(g_Header->getWidth(), 2);
            b[3] = g_Header->convertFrom16To8(g_Header->getWidth(), 1);
            b[4] = g_Header->convertFrom16To8(g_Header->getHeight(), 2);
            b[5] = g_Header->convertFrom16To8(g_Header->getHeight(), 1);
            b[7] = (char) g_Header->getComplex();
            b[6] = (char) 0x00;
            l_File.write(b,8);
            if(!l_File || l_File.fail()) return false;
        }
            else return false;

        if(g_Header->getIInterleave()==g_Header->getInterleave()) {
            if(buildCharChain(l_File,1,g_Data)){
                l_File.close();
                return true;
            }
               else return false;
        }
        else if(g_Header->getIInterleave()>1) rebuildDataPixel();

        if(buildCharChain(l_File,g_Header->getInterleave(),g_Data)){
            l_File.close();
            return true;
        }
        else return false;
    }

    bool buildCharChain(ofstream & p_File,int p_Interleave,vector<Pixel> & p_Data){
        if(p_Interleave==0){
            p_Data.clear();
            return true;
        }

        vector<Pixel> l_Data;
        bool lW= g_Header->getWEnd();
        bool lE= g_Header->getEndian();

        for ( auto &l_Pixel : p_Data){
            if(l_Pixel.getX()%p_Interleave==0 && l_Pixel.getY()%p_Interleave==0){
                for (int i = 0; i < g_Header->getChannelPixel(); ++i) {
                    if(lE==lW)
                        for (int j = 0; j < g_Header->getBitChannel()/8; ++j) {
                                p_File.write(l_Pixel.getDataByIndex(i*g_Header->getBitChannel()/8+j),1);
                            if(!p_File || p_File.fail()) return false;
                    }
                    else
                        for (int j = g_Header->getBitChannel()/8; j; --j) {
                                p_File.write(l_Pixel.getDataByIndex(i*g_Header->getBitChannel()/8+j-1),1);
                            if(!p_File || p_File.fail()) return false;
                    }
                }
            }else l_Data.push_back(l_Pixel);

        }
        p_Data.clear();

        buildCharChain(p_File,p_Interleave/2,l_Data);

        return true;
    }

    void rebuildDataPixel(){

        vector<Pixel> l_RebildBuffer;

        for (int i = 0; i < g_Header->getHeight() ; ++i) {
            for (int j = 0; j < g_Header->getWidth() ; ++j) {
                l_RebildBuffer.push_back(Pixel(i,j,NULL));
            }
        }

        rebuildRecursively(g_Header->getIInterleave(),0,1,g_Data,l_RebildBuffer);

    }

    void rebuildRecursively(int p_Interleave,int p_Cnt,int p_D,vector<Pixel> & p_In,vector<Pixel> & p_Out){
        if(p_Interleave==0){ g_Data.clear();
            g_Data = p_Out;
            return ;}


        for (int i = 0; i < g_Header->getHeight(); i+=p_Interleave) {
            for (int j = 0; j < g_Header->getWidth(); j+=p_Interleave) {
                if(i%p_Interleave==0 && j%p_Interleave==0 && p_D==1)
                    p_Out[i*g_Header->getWidth()+j].setBuffer(p_In[p_Cnt++].getBuffer());
                else if((i%(2*p_Interleave)==0 && j%(2*p_Interleave)==0))
                    continue;
                else p_Out[i*g_Header->getWidth()+j].setBuffer(p_In[p_Cnt++].getBuffer());
            }
        }
        rebuildRecursively(p_Interleave/2,p_Cnt,p_D+1,p_In,p_Out);
    }

    size_t getFileLenght(const char * p_FileName){
        ifstream l_FileIn(p_FileName,ios::in|ios::binary);
        size_t l_FileLen=0;

        if(!l_FileIn || l_FileIn.fail()) return 0;
            l_FileIn.seekg(0, l_FileIn.end);
        if(!l_FileIn || l_FileIn.fail()) return 0;
            l_FileLen = l_FileIn.tellg();
        if(!l_FileIn || l_FileIn.fail()) return 0;
            l_FileIn.seekg(0, l_FileIn.beg);


        return l_FileLen;

    }

};


bool recodeImage ( const char  * srcFileName,
                   const char  * dstFileName,
                   int           interleave,
                   uint16_t      byteOrder )
{
    Image l_Input;

    if(!(interleave==1 || interleave==2 || interleave==4 || interleave==8 || interleave==16 || interleave==32 || interleave==64) || interleave>65)return false;

    if(!l_Input.setImage(srcFileName,interleave,byteOrder))
        return false;
    if(!l_Input.writeToFile(dstFileName))
        return false;

    return true;
}

#ifndef __PROGTEST__


bool identicalFiles ( const char * fileName1,
                      const char * fileName2 )
{
    ifstream l_File1(fileName1);
    ifstream l_File2(fileName2);
    size_t l_FileLen=0;size_t l_FileLen2=0;

    l_File1.seekg(0,l_File1.end);
    l_FileLen = l_File1.tellg();
    l_File1.seekg(0,l_File1.beg);

    char * buff = new char[l_FileLen];

    l_File2.seekg(0,l_File2.end);
    l_FileLen2 = l_File2.tellg();
    l_File2.seekg(0,l_File2.beg);

    char * b = new char[l_FileLen2];

    l_File1.read(buff,l_FileLen);
    l_File2.read(b,l_FileLen2);

    for (int i = 0; i < (int)l_FileLen; ++i) {
        if(buff[i]!=b[i])
            return false;
    }

    delete [] buff;
    delete [] b;

    return true;
}

int main ( void )
{
   assert ( recodeImage ( "input_00.img", "output_00.img", 1, ENDIAN_LITTLE )
             && identicalFiles ( "output_00.img", "ref_00.img" ) );

    assert ( recodeImage ( "input_01.img", "output_01.img", 8, ENDIAN_LITTLE )
             && identicalFiles ( "output_01.img", "ref_01.img" ) );

    assert ( recodeImage ( "input_02.img", "output_02.img", 8, ENDIAN_LITTLE )
             && identicalFiles ( "output_02.img", "ref_02.img" ) );

    assert ( recodeImage ( "input_03.img", "output_03.img", 2, ENDIAN_LITTLE )
             && identicalFiles ( "output_03.img", "ref_03.img" ) );

    assert ( recodeImage ( "input_04.img", "output_04.img", 1, ENDIAN_LITTLE )
             && identicalFiles ( "output_04.img", "ref_04.img" ) );

    assert ( recodeImage ( "input_05.img", "output_05.img", 1, ENDIAN_LITTLE )
             && identicalFiles ( "output_05.img", "ref_05.img" ) );

    assert ( recodeImage ( "input_06.img", "output_06.img", 8, ENDIAN_LITTLE )
             && identicalFiles ( "output_06.img", "ref_06.img" ) );

    assert ( recodeImage ( "input_07.img", "output_07.img", 4, ENDIAN_LITTLE )
             && identicalFiles ( "output_07.img", "ref_07.img" ) );

    assert ( recodeImage ( "input_08.img", "output_08.img", 8, ENDIAN_LITTLE )
             && identicalFiles ( "output_08.img", "ref_08.img" ) );

    /*assert ( recodeImage ( "in_2652300.bin", "out_2652300.bin", 64, ENDIAN_LITTLE )
             && identicalFiles ( "out_2652300.bin", "ref_2652300.bin" ) );

    assert ( !recodeImage ( "in_2652296.bin", "out_2652296.bin", 8, ENDIAN_LITTLE )
              );

    assert ( !recodeImage ( "in_2655187.bin", "out_2655187.bin", 64, ENDIAN_LITTLE )
              );
    assert ( recodeImage ( "test.img", "out.img", 64, ENDIAN_LITTLE )
    );
*/



    assert ( ! recodeImage ( "input_09.img", "output_09.img", 1, ENDIAN_LITTLE ) );

    assert ( ! recodeImage ( "input_10.img", "output_10.img", 5, ENDIAN_LITTLE ) );

    // extra inputs (optional & bonus tests)
    assert ( recodeImage ( "extra_input_00.img", "extra_out_00.img", 8, ENDIAN_LITTLE )
             && identicalFiles ( "extra_out_00.img", "extra_ref_00.img" ) );
    assert ( recodeImage ( "extra_input_01.img", "extra_out_01.img", 4, ENDIAN_BIG )
             && identicalFiles ( "extra_out_01.img", "extra_ref_01.img" ) );
    assert ( recodeImage ( "extra_input_02.img", "extra_out_02.img", 16, ENDIAN_BIG )
             && identicalFiles ( "extra_out_02.img", "extra_ref_02.img" ) );
    assert ( recodeImage ( "extra_input_03.img", "extra_out_03.img", 1, ENDIAN_LITTLE )
             && identicalFiles ( "extra_out_03.img", "extra_ref_03.img" ) );
    assert ( recodeImage ( "extra_input_04.img", "extra_out_04.img", 8, ENDIAN_LITTLE )
             && identicalFiles ( "extra_out_04.img", "extra_ref_04.img" ) );
    assert ( recodeImage ( "extra_input_05.img", "extra_out_05.img", 4, ENDIAN_LITTLE )
             && identicalFiles ( "extra_out_05.img", "extra_ref_05.img" ) );
    assert ( recodeImage ( "extra_input_06.img", "extra_out_06.img", 16, ENDIAN_BIG )
             && identicalFiles ( "extra_out_06.img", "extra_ref_06.img" ) );
    assert ( recodeImage ( "extra_input_07.img", "extra_out_07.img", 1, ENDIAN_BIG )
             && identicalFiles ( "extra_out_07.img", "extra_ref_07.img" ) );
    /*assert ( recodeImage ( "extra_input_08.img", "extra_out_08.img", 8, ENDIAN_LITTLE )
             && identicalFiles ( "extra_out_08.img", "extra_ref_08.img" ) );
    assert ( recodeImage ( "extra_input_09.img", "extra_out_09.img", 4, ENDIAN_LITTLE )
             && identicalFiles ( "extra_out_09.img", "extra_ref_09.img" ) );
    assert ( recodeImage ( "extra_input_10.img", "extra_out_10.img", 16, ENDIAN_BIG )
             && identicalFiles ( "extra_out_10.img", "extra_ref_10.img" ) );
    assert ( recodeImage ( "extra_input_11.img", "extra_out_11.img", 1, ENDIAN_BIG )
             && identicalFiles ( "extra_out_11.img", "extra_ref_11.img" ) );*/
    return 0;
}
#endif /* __PROGTEST__ */