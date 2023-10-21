
#include "DataStream.h"
#include <array>
#include <assert.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <set>

using namespace std;
using namespace lumos;


void DataStream::reserve(int len)   
    {
        int size = buf.size();
        int cap = buf.capacity();

        if(size + len > cap)    
        {
            while(size + len > cap)
            {
                if(cap == 0)
                {
                    cap = 1;
                }
                else
                {
                    cap *= 2;
                }
            }
            buf.reserve(cap);
        }
    }

    void DataStream::show() const
    {
        int size = buf.size();
        cout << "data size = " << size << endl;
      //  for(int j = 0; j < size; j++)
        {
       //     cout << (int)buf[j] << endl;
        }

       // cout << (DataType)buf[0] << endl;

        int i = 0;
        while(i < size)
        {
            //DataType type = (DataType)buf[i];
            //cout << type << endl;
            switch((DataType)buf[i])
            {
                case DataType::BOOL:
                {
                    if((int)buf[++i] == 0)
                    {
                        cout << "false" << endl;
                    }
                    else
                    {
                        cout << "true" << endl;
                    }
                    i++;
                    break; 
                }
                case DataType::CHAR:
                {
                    cout << buf[++i];
                    i++;
                    break; 
                }
                case DataType::INT32:
                {
                    cout << *((int32_t*)(&buf[++i]));
                    i += 4;
                    break; 
                }
                case DataType::INT64:
                {
                    cout << *((int64_t*)(&buf[++i]));
                    i += 8;
                    break; 
                }
                case DataType::FLOAT:
                {
                    cout << *((float*)(&buf[++i]));
                    i += 4;
                    break; 
                }
                case DataType::DOUBLE:
                {
                    cout << *((double*)(&buf[++i]));
                    i += 8;
                    break; 
                }
                case DataType::STRING:
                {
                    if((DataType)buf[++i] == DataType::INT32)
                    {
                        int len = *((int32_t*)(&buf[++i]));
                        i += 4;
                        cout << string(&buf[i],len);
                        i += len;
                    }
                    else
                    {
                        throw logic_error("parse string error");
                    }
                    break; 
                }
                default:
                    break; 
            }
        }
    }

    void DataStream::save(const char* fname)
    {
        FILE *fp = NULL;
        if((fp = fopen(fname,"wb")) == NULL)
        {
            fprintf(stdout,"fail to open file!\n");
            exit(EXIT_FAILURE);
        }
        int32_t len = buf.size();
        write(len);

        cout << "len = " << len << endl;
        cout << "size of buf: " << buf.size() << endl;
        fwrite(&buf,buf.size(),1, fp);
        fclose(fp);
        return;
    }

    void DataStream::load(const char* fname)
    {
        FILE *fp = NULL;
        if((fp = fopen(fname,"rb")) == NULL)
        {
            fprintf(stdout,"fail to open file!\n");
            exit(EXIT_FAILURE);
        }
        
        int len = 0;
        reserve(sizeof(int32_t));
        fseek(fp,-6L,SEEK_END);
        fread(&buf[pos],sizeof(char),5,fp);
        read(len);
        buf.resize(len);
        cout << "len = " << (int)len << endl;
        cout << "len size = " << sizeof(len) << endl;
        fseek(fp,0L,SEEK_SET);
       // reserve((int)len);

        while(fgetc(fp) != EOF)
        {
            fread(&buf,len,1,fp);
        }
        fclose(fp);

        return;
    }

    void DataStream::write(const char *data, int len)
    {
        reserve(len);

        //将数据写入buf
        int size = buf.size();
        buf.resize(size+len);   //加大尺寸
        memcpy(&buf[size],data,len);

    }
    void DataStream::write(bool value)
    {
        char type = DataType::BOOL;
        write((char*)&type,sizeof(char));   //写入数据类型
        write((char*)&value,sizeof(char));
    }
    void DataStream::write(char value)
    {
        char type = DataType::CHAR;
        write((char*)&type,sizeof(char));
        write((char*)&value,sizeof(char));
    }
    void DataStream::write(int32_t value)
    {
        char type = DataType::INT32;
        write((char*)&type,sizeof(char));
        write((char*)&value,sizeof(int32_t));
    }
    void DataStream::write(int64_t value)
    {
        char type = DataType::INT64;
        write((char*)&type,sizeof(char));
        write((char*)&value,sizeof(int64_t));
    }
    void DataStream::write(float value)
    {
        char type = DataType::FLOAT;
        write((char*)&type,sizeof(char));
        write((char*)&value,sizeof(float));
    }
    void DataStream::write(double value)
    {
        char type = DataType::DOUBLE;
        write((char*)&type,sizeof(char));
        write((char*)&value,sizeof(double));
    }
    void DataStream::write(const char *value)
    {
        char type = DataType::STRING;
        write((char*)&type,sizeof(char));
        int len = strlen(value);
        write(len); //写入长度
        write(value,len);
    }
    void DataStream::write(const string value)
    {
        char type = DataType::STRING;
        write((char*)&type,sizeof(char));
        int len = value.size();
        write(len);
        write(value.data(),len);
    }

    bool DataStream::read(bool &value)
    {
        if(buf[pos] != DataType::BOOL)
        {
            return false;
        }
        ++pos;
        value = buf[pos];
        ++pos;
        return true;
    }

    bool DataStream::read(char &value)
    {
        if(buf[pos] != DataType::CHAR)
        {
            return false;
        }
        ++pos;
        value = buf[pos];
        ++pos;
        return true;
    }

    bool DataStream::read(int32_t &value)
    {
        if(buf[pos] != DataType::INT32)
        {
            return false;
        }
        ++pos;
        value = *((int32_t*)(&buf[pos]));
        pos += 4;
        return true;
    }

    bool DataStream::read(int64_t &value)
    {
        if(buf[pos] != DataType::INT64)
        {
            return false;
        }
        ++pos;
        value = *((int64_t*)(&buf[pos]));
        pos += 8;
        return true;
    }

    bool DataStream::read(float &value)
    {
        if(buf[pos] != DataType::FLOAT)
        {
            return false;
        }
        ++pos;
        value = *((float*)(&buf[pos]));
        pos += 4;
        return true;
    }

    bool DataStream::read(double &value)
    {
        if(buf[pos] != DataType::DOUBLE)
        {
            return false;
        }
        ++pos;
        value = *((double*)(&buf[pos]));
        pos += 8;
        return true;
    }

    bool DataStream::read(string &value)
    {
        if(buf[pos] != DataType::STRING)
        {
            return false;
        }
        ++pos;
        int len;
        read(len);
        if(len < 0)
        {
            return false;
        }
        value.assign((char *)&(buf[pos]),len);
        pos += len;
        return true;
    }