#pragma once

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

namespace type
{
    template <typename T>
    using remove_cvref =
    typename std::remove_cv<typename std::remove_reference<T>::type>;

    template <typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;
}

namespace lumos
{
    class DataStream        //序列化和反序列化功能
    {

    public:
        enum DataType       //序列化类型
        {
            //基本类型
            BOOL = 0,
            CHAR,
            INT32,
            INT64,
            FLOAT,
            DOUBLE,
            STRING,
            VECTOR,
            STACK,
            MAP,
            QUEUE,
            SET
        };

        DataStream(): pos(0) {}
        ~DataStream(){}

        void show() const;    //显示结果



        void save(const char* fname);

        void load(const char* fname);

        /***************************************
                          写入
        ****************************************/
        // template <typename T, typename T_ = typename type::remove_cvref_t<T>>
        // static T_ &get_writable(T &&t);

        // template <typename T, typename T_ = typename type::remove_cvref_t<T>>
        // static void get_type(T &&t);

        template<typename T>
        void write(T value);

        void write(const char *data, int len);    //通过WRITE写入VECTOR
        void write(bool value);     //序列化BOOL类型
        void write(char value);     //序列化CHAR类型
        void write(int32_t value);
        void write(int64_t value);
        void write(float value);
        void write(double value);
        void write(const char* value);      // stringC
        void write(const string value);     //stringC++


        template<typename T>
        void write(vector<T> &value);

        template<typename T>
        void write(stack<T> &value);

        template<typename T1,typename T2>
        void write(map<T1,T2> &value);

        template<typename T>
        void write(queue<T> &value);

        template<typename T>
        void write(deque<T> &value);

        template<typename T>
        void write(set<T> &value);

        //template<typename type_t>
        //void write(type_t obj);

        /****************************************
                           解码
        *****************************************/

        bool read(bool& value); //bool 解码
        bool read(char& value);
        bool read(int32_t& value);
        bool read(int64_t& value);
        bool read(float& value);
        bool read(double& value);
        bool read(string& value);

        template<typename T>
        bool read(T &value);
        
        template <typename T>
        bool read(vector<T> &value);

        template<typename T>
        bool read(stack<T> &value);

        template<typename T1,typename T2>
        bool read(map<T1,T2> &value);

        template<typename T>
        bool read(queue<T> &value);

        template<typename T>
        bool read(deque<T> &value);

        template<typename T>
        bool read(set<T> &value);

        // template<typename T,typename T_ = typename type::remove_cvref_t<T>>
        // void read_byte(T value);



        template<typename type_t>
        DataStream& operator << (type_t value);

        template<typename type_t>
        DataStream& operator >> (type_t &value);

    private:
        void reserve(int len);  //判断大小并扩容

    private:
        std::vector<char> buf;    //缓存区
        int pos;    //记录当前解码位置

    };


    template <typename T>
    void DataStream::write(T value)
    {
        auto &val = value;

        FILE *fp = NULL;

        if((fp = fopen("Serialize.bin","wb+")) == NULL)
        {
            fprintf(stdout,"fail to open file!\n");
            exit(0);
        }

        fwrite(&value,1,sizeof(T),fp);
        fclose(fp);
    }

    //serialize vector
    template <typename T>
    void DataStream::write(vector<T> &value)
    {
        auto &val = value;
        char type = DataType::VECTOR;
        //int number = count;
        //count++;
        write((char*)&type,sizeof(char));   //1
        int len = val.size();
        write(len);     //5
        if(val.empty())
        {
           pos++;
          // stl_size[number] = 0;
           return; 
        }
        
        //stl_size[number++] = sizeof(val);

        for(size_t i = 0; i < val.size(); i++)
        {
            write(val[i]);
        }
        return;
    }

    template<typename T>
    void DataStream::write(stack<T> &value)
    {
        auto &val = value;
        char type = DataType::STACK;
        //int number = count;
        //count++;
        write((char*)&type,sizeof(char));   //1
        int len = val.size();
        write(len);     //5
        if(val.empty())
        {
           pos++;
          // stl_size[number] = 0;
           return; 
        }
        while(!val.empty())
        {
            write(val.top());
            val.pop();
        }

        return;
    }


    //serialize map
    template<typename T1,typename T2>
    void DataStream::write(map<T1,T2> &value)
    {
        auto &val = value;
        char type = DataType::MAP;
        //int number = count;
        //count++;
        write((char*)&type,sizeof(char));   //1
        int len = val.size();
        write(len);     //5
        if(val.empty())
        {
           pos++;
          // stl_size[number] = 0;
           return; 
        }
        
        //stl_size[number++] = sizeof(val);

        auto it = val.begin();
        while(it != val.end())
        {
            write(it->first);
            write(it->second);
            it++;
        }

        return;
    }

    template<typename T>
    void DataStream::write(queue<T> &value)
    {
        auto &val = value;
        char type = DataType::QUEUE;
        //int number = count;
        //count++;
        write((char*)&type,sizeof(char));   //1
        int len = val.size();
        write(len);     //5
        if(val.empty())
        {
           pos++;
          // stl_size[number] = 0;
           return; 
        }
        
        //stl_size[number++] = sizeof(val);

        while(!val.empty())
        {
            write(val.front());
            val.pop();
        }

        return;
    }


    template<typename T>
    void DataStream::write(set<T> &value)
    {
        auto &val = value;
        char type = DataType::SET;

        write((char*)&type,sizeof(char));   //1
        int len = val.size();
        write(len);     //5
        if(val.empty())
        {
           pos++;
          // stl_size[number] = 0;
           return; 
        }
        
        //stl_size[number++] = sizeof(val);
        set<int>::iterator it = val.begin();
        for(it = val.begin(); it != val.end(); it++)
        {
            write(*it);
        }
        return;
    }

    template<typename T>
    bool DataStream::read(T &value)
    {
        FILE *fp = NULL;
        auto &val = value;
        if((fp = fopen("Serialize.bin","rb+")) == NULL)
        {
            fprintf(stdout,"fail to open file!\n");
            //exit(0);
            return false;
        }
        value

        fread(&val,sizeof(T),1,fp);
        fclose(fp);
        return true;
    }


    //deseialize vector
    template <typename T>
    bool DataStream::read(vector<T> &value)
    {
        if(buf[pos] != DataType::VECTOR)
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
        for(int i = 0; i < len; i++)
        {
            T val;
            read(val);
            value.push_back(val);
        }
        return true;           
    }

    //deserialize stack
    template<typename T>
    bool DataStream::read(stack<T> &value)
    {
        if(buf[pos] != DataType::STACK)
        {
            return false;
        }
        ++pos;

        stack<T> tempt;

        int len;
        read(len);
        if(len < 0)
        {
            return false;
        }
        for(int i = 0; i < len; i++)
        {
            T val;
            read(val);
            tempt.push(val);
        }
        while(!tempt.empty())
        {
            value.push(tempt.top());
            tempt.pop();
        }

        return true;     

        
    }

    //deserialize map
    template <typename T1,typename T2>
    bool DataStream::read(map<T1,T2> &value)
    {
        if(buf[pos] != DataType::MAP)
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
        for(int i = 0; i < len; i++)
        {
            T1 val1;
            T2 val2;
            read(val1);
            read(val2);
            value.insert(pair<T1,T2>(val1,val2));
        }
        return true;           
    }

    template<typename T>
    bool DataStream::read(queue<T> &value)
    {
        if(buf[pos] != DataType::QUEUE)
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

        T val;
        for(int i = 0; i < len; i++)
        {
            read(val);
            value.push(val);
        }
        return true;      
    }

    template<typename T>
    bool DataStream::read(set<T> &value)
    {
        if(buf[pos] != DataType::SET)
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

        T val;
        for(int i = 0; i < len; i++)
        {
            read(val);
            value.insert(val);
        }
        return true;      
    }

 

    template <typename type_t>
    DataStream &DataStream::operator<<(type_t value)
    {
        write(value);
        return *this;
    }

    template <typename type_t>
    DataStream &DataStream::operator>>(type_t &value)
    {
        read(value);
        return *this;
    }


}
