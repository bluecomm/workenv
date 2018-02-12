#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <time.h>

using namespace std;
using namespace boost::property_tree;


const string file_path = "./test.txt";

void write_json_data_into_file(void)
{
    printf("%s\n","write json data");

    boost::property_tree::ptree root, items;

    boost::property_tree::ptree item1;
    item1.put("ID","1");
    item1.put("Name","wang");
    items.push_back(std::make_pair("1",item1));

    boost::property_tree::ptree item2;
    item2.put("ID","2");
    item2.put("Name","zhang");
    items.push_back(std::make_pair("2",item2));

    root.put_child("users",items);
    boost::property_tree::write_json(file_path,root);
}

void read_json_data_from_file(void)
{
    printf("%s\n","read json data");
    boost::property_tree::ptree root;
    boost::property_tree::ptree items;
    boost::property_tree::read_json<boost::property_tree::ptree>(file_path,root);

    items = root.get_child("users");
    for(boost::property_tree::ptree::iterator it=items.begin(); it != items.end(); ++it)
    {
        string key=it->first;//key ID
        string ID=it->second.get<string>("ID");
        string Name=it->second.get<string>("Name");
        cout<<"key: "<<key.c_str()<<'\t';
        printf("ID: %s    Name: %s",ID.c_str(), Name.c_str());
        cout<<endl;
        cout<<"ID:"<<ID<<'\t'<<"Name:"<<Name<<endl;
    }
    cout<<"success"<<endl;
}

void write_json_data_into_string(void)
{
    boost::property_tree::ptree item;
    item.put("a","2");
    std::stringstream is;
    boost::property_tree::write_json(is,item);
    std::string s = is.str();
    cout<<"json s:"<<s<<endl;
}

void read_json_data_from_string(void)
{
    /*
        C++ 中 字符串形式的json串 需要使用 \ 转义 双引号
    */
    std::string str_json = "{\"count\":10,\"people\":[{ \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" },{ \"firstName\": \"Jason\", \"lastName\":\"Hunter\", \"email\": \"bbbb\"},{ \"firstName\": \"Elliotte\", \"lastName\":\"Harold\", \"email\": \"cccc\" }]}";
    std::stringstream str_stream(str_json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(str_stream,root);
    root.put("upid","001");

    // 插入一个数组
    boost::property_tree::ptree exif_array;
    boost::property_tree::ptree array1, array2, array3;
    array1.put("Make", "NIKON");
    array2.put("DateTime", "2011:05:31 06:47:09");
    array3.put("Software", "Ver.1.01");

    //   exif_array.push_back(std::make_pair("Make", "NIKON"));
    //   exif_array.push_back(std::make_pair("DateTime", "2011:05:31 06:47:09"));
    //   exif_array.push_back(std::make_pair("Software", "Ver.1.01"));

    exif_array.push_back(std::make_pair("", array1));
    exif_array.push_back(std::make_pair("", array2));
    exif_array.push_back(std::make_pair("", array3));

    root.put_child("exifs", exif_array);
    std::stringstream str_stream_temp;
    boost::property_tree::write_json(str_stream_temp, root);
    //write_json(str_stream_temp, root);
    std::string str = str_stream_temp.str();
    cout<<str<<endl;
}


void testMsg(void)
{
    string str_json = "{\"TEST\":\"\",\"MSG\":\"130\",\"TKT\":[{\"DTYP\":\"T\",\"STOT\":\"1\",\"SNUM\":\"1\",\"CPN\":[{\"CNBR\":\"1\",\"DDAT\":\"090117\",\"DTME\":\"1205\",\"ADAT\":\"090117\",\"ATME\":\"1340\",\"ORIG\":\"TSA\",\"DEST\":\"PVG\",\"ALC1\":\"FM\",\"FLTN\":\"802\",\"CLAS\":\"Y\",\"FSTN\":\"OK\",\"FBAS\":\"Y\",\"BAGA\":\"20K\"}]}]}";
    std::stringstream str_stream(str_json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(str_stream,root);
    boost::property_tree::ptree::iterator root_it = root.begin();
    for(; root_it != root.end(); ++root_it)
    {
        string key = root_it->first;
        if("TKT" == key)
        {
            boost::property_tree::ptree tkt_node = root.get_child(key);
            boost::property_tree::ptree::iterator tkt_node_it = tkt_node.begin();
            for(; tkt_node_it != tkt_node.end(); ++tkt_node_it)
            {
                boost::property_tree::ptree tkt = tkt_node_it->second;
                boost::property_tree::ptree::iterator tkt_it = tkt.begin();
                for(; tkt_it != tkt.end(); ++tkt_it)
                {
                    string tkt_key = tkt_it->first;
                    if("CPN" == tkt_key)
                    {
                        boost::property_tree::ptree cpn_node = tkt.get_child(tkt_key);
                        boost::property_tree::ptree::iterator cpn_node_it = cpn_node.begin();
                        for(; cpn_node_it != cpn_node.end(); ++cpn_node_it)
                        {
                            boost::property_tree::ptree cpn = cpn_node_it->second;
                            boost::property_tree::ptree::iterator cpn_it = cpn.begin();
                            for(; cpn_it != cpn.end();++cpn_it)
                            {
                                string cpn_key = cpn_it->first;
                                string cpn_val = cpn.get<string>(cpn_key);
                                cout<<cpn_key<<":"<<cpn_val<<endl;
                            }
                        }
                    }
                    else
                    {
                        string tkt_val = tkt.get<string>(tkt_key);
                        cout << tkt_key << ":"<<tkt_val<<endl;
                    }
                }
            }
        }
        else
        {
            string val = root.get<string>(key);
            cout << key <<":"<< val <<endl;
            //Sleep(1000);
        }

    }
}

void bianli_json(void)
{
    string json_string="{\"-f\": \"/usr/reservedfff_dir\", \"-s\": \"/usr/reservedddd_dir\"}";

    string str_head;
    string str_node_val;
    boost::property_tree::ptree pt,p1,p2;

    stringstream stream(json_string);
    try
    {
        boost::property_tree::read_json<boost::property_tree::ptree>(stream, pt);
        cout<<"parsing ok\n"<<endl;
        for (boost::property_tree::ptree::iterator ita = pt.begin(); ita != pt.end(); ++ita)
        {
            cout<<"first:"<<ita->first<<endl;
            str_node_val = pt.get<string>(ita->first);
            cout<<str_node_val<<endl;
        }
    }
    catch(std::runtime_error& e)
    {
        std::cout<<e.what()<<endl;
    }
}

void test(void)
{
    string str_json = "{\"TEST\":\"\",\"MSG\":\"130\",\"TKT\":[{\"DTYP\":\"T\",\"STOT\":\"1\",\"SNUM\":\"1\",\"CPN\":[{\"CNBR\":\"1\",\"DDAT\":\"090117\",\"DTME\":\"1205\",\"ADAT\":\"090117\",\"ATME\":\"1340\",\"ORIG\":\"TSA\",\"DEST\":\"PVG\",\"ALC1\":\"FM\",\"FLTN\":\"802\",\"CLAS\":\"Y\",\"FSTN\":\"OK\",\"FBAS\":\"Y\",\"BAGA\":\"20K\"}]}]}";
    std::stringstream str_stream(str_json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(str_stream,root);

    boost::property_tree::ptree tkt = root.get_child("TKT");
    stringstream tkt_stream;
    boost::property_tree::write_json(tkt_stream, tkt);
    cout<<tkt_stream.str()<<endl; //通过打印可以看到 first 为空

    for(boost::property_tree::ptree::iterator it = tkt.begin(); it!=tkt.end();++it)
    {
        boost::property_tree::ptree inner = it->second; //first为空
        //string DTYP_val = inner.get<string>("DTYP");
        //cout << "DTYP" <<":"<< DTYP_val<<endl;

        boost::property_tree::ptree::iterator it_tkt = inner.begin();
        for(; it_tkt != inner.end(); ++it_tkt)
        {
            string innet_key = it_tkt->first;
            cout<<innet_key<<":"<<inner.get<string>(innet_key)<<endl;
        }
        break;
    }

    //    boost::property_tree::ptree pt,pptt,ttt;
    //    string s = "{\"data\":[{\"id\":1,\"name\":\"chen\"},{\"id\":2,\"name\":\"zhang\"}]}";
    //    stringstream stream(s);
    //    read_json(stream, pt);

    //    pptt = pt.get_child("data");
    //    for (boost::property_tree::ptree::iterator it = pptt.begin(); it != pptt.end(); ++it)
    //    {
    //        ttt = it->second; //first为空
    //        cout<<"id="<<ttt.get<string>("id")<<endl;
    //        cout<<"name="<<ttt.get<string>("name")<<endl;
    //    }
}

void parseMsg_1(void)
{
    string msg_str = "{\"MSG\":\"130\",\"CRSC\":\"MU\",\"CRSL\":\"BJS\",\"TAID\":\"08692057\",\"IHID\":\"PEK112\",\"ORGT\":\"A\",\"CRSN\":\"9995\",\"IHIB\":\"PEK112\",\"IDTP\":\"B\",\"PNR1\":\"NYFY37\",\"PNR2\":\"MU\",\"SURN\":\"zhang\",\"GIVN\":\"san\",\"FMID\":\"PPG19739941\",\"DTIS\":\"030117\",\"JORG\":\"TPE\",\"JDST\":\"MAD\",\"FAMT\":\"1560.00\",\"FCUR\":\"CNY\",\"TAMT\":\"1660.00\",\"TCUR\":\"CNY\",\"FANF\":\"H/1560.00/CNY+G/00:00/CNY\",\"FOPF\":\"CC\",\"TAXF\":\"T/CNY/50.00/CN+T/CNY/50.00/YQ\",\"FACF\":\"M/09JAN17TSA FM PVG MU JFK/-GRU JJ MAD1560.00CNY1560.00END\",\"TKT\":[{\"DTYP\":\"T\",\"STOT\":\"1\",\"SNUM\":\"1\",\"CPN\":[{\"CNBR\":\"1\",\"DDAT\":\"090117\",\"DTME\":\"1205\",\"ADAT\":\"090117\",\"ATME\":\"1340\",\"ORIG\":\"TSA\",\"DEST\":\"PVG\",\"ALC1\":\"FM\",\"FLTN\":\"802\",\"CLAS\":\"Y\",\"FSTN\":\"OK\",\"FBAS\":\"Y\",\"BAGA\":\"20K\"},{\"CNBR\":\"1\",\"DDAT\":\"090117\",\"DTME\":\"1205\",\"ADAT\":\"090117\",\"ATME\":\"1340\",\"ORIG\":\"TSA\",\"DEST\":\"PVG\",\"ALC1\":\"FM\",\"FLTN\":\"802\",\"CLAS\":\"Y\",\"FSTN\":\"OK\",\"FBAS\":\"Y\",\"BAGA\":\"20K\"}]},{\"DTYP\":\"T\",\"STOT\":\"1\",\"SNUM\":\"1\",\"CPN\":[{\"CNBR\":\"1\",\"DDAT\":\"090117\",\"DTME\":\"1205\",\"ADAT\":\"090117\",\"ATME\":\"1340\",\"ORIG\":\"TSA\",\"DEST\":\"PVG\",\"ALC1\":\"FM\",\"FLTN\":\"802\",\"CLAS\":\"Y\",\"FSTN\":\"OK\",\"FBAS\":\"Y\",\"BAGA\":\"20K\"}]}]}";
    stringstream msg_ss(msg_str);
    boost::property_tree::ptree msg;
    boost::property_tree::read_json(msg_ss, msg);
    boost::property_tree::ptree::iterator msg_it = msg.begin();
    for(; msg_it != msg.end(); ++msg_it)
    {
        string msg_key = msg_it->first;
        string val = "";
        if("TKT" == msg_key)
        {
            boost::property_tree::ptree tkt_node = msg.get_child(msg_key);
            boost::property_tree::ptree::iterator tkt_node_it = tkt_node.begin();
            for(; tkt_node_it != tkt_node.end(); ++tkt_node_it)
            {
                boost::property_tree::ptree tkt = tkt_node_it->second;
                boost::property_tree::ptree::iterator tkt_it = tkt.begin();
                for(; tkt_it != tkt.end(); ++tkt_it)
                {
                    string tkt_key = tkt_it->first;
                    if("CPN" == tkt_key)
                    {
                        boost::property_tree::ptree cpn_node = tkt.get_child(tkt_key);
                        boost::property_tree::ptree::iterator cpn_node_it = cpn_node.begin();
                        for(; cpn_node_it != cpn_node.end(); ++cpn_node_it)
                        {
                            boost::property_tree::ptree cpn = cpn_node_it->second;
                            boost::property_tree::ptree::iterator cpn_it = cpn.begin();
                            for(; cpn_it != cpn.end(); ++cpn_it)
                            {
                                string cpn_key = cpn_it->first;
                                val = cpn.get<string>(cpn_key);
                                cout << cpn_key << ":" << val <<endl;
                            }
                        }
                    }
                    else
                    {
                        val = tkt.get<string>(tkt_key);
                        cout<< tkt_key << ":" << val <<endl;
                    }
                }
            }
        }
        else
        {
            val = msg.get<string>(msg_key);
            cout<< msg_key<<":"<<val<<endl;
        }
    }

}

void parseMsg(void)
{
    string msg_str = "{\"MSG\":\"MSG\",\"CRSC\":\"CRSC\",\"CRSL\":\"CRSL\",\"TAID\":\"TAID\",\"IHID\":\"IHID\",\"ORGT\":\"ORGT\",\"CRSN\":\"CRSN\",\"IHIB\":\"IHIB\",\"IDTP\":\"IDTP\",\"PNR1\":\"PNR1\",\"PNR2\":\"PNR2\",\"TIF\":[{\"SURN\":\"SURN\",\"GIVN\":\"GIVN\",\"FMID\":\"FMID\",\"INFP\":\"INFP\",\"UMCH\":\"UMCH\",\"JORG\":\"JORG\",\"JDST\":\"JDST\",\"FAMT\":\"FAMT\",\"FCUR\":\"FCUR\",\"TAMT\":\"TAMT\",\"TCUR\":\"TCUR\",\"FANF\":\"FANF\",\"FOPF\":\"FOPF\",\"TAXF\":\"TAXF\",\"TKT\":[{\"TKNB\":\"TKNB\",\"STOT\":\"STOT\",\"SNUM\":\"SNUM\",\"CPN\":[{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"},{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"}]},{\"TKNB\":\"TKNB\",\"STOT\":\"STOT\",\"SNUM\":\"SNUM\",\"CPN\":[{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"},{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"}]}]},{\"SURN\":\"SURN\",\"GIVN\":\"GIVN\",\"FMID\":\"FMID\",\"INFP\":\"INFP\",\"UMCH\":\"UMCH\",\"JORG\":\"JORG\",\"JDST\":\"JDST\",\"FAMT\":\"FAMT\",\"FCUR\":\"FCUR\",\"TAMT\":\"TAMT\",\"TCUR\":\"TCUR\",\"FANF\":\"FANF\",\"FOPF\":\"FOPF\",\"TAXF\":\"TAXF\",\"TKT\":[{\"TKNB\":\"TKNB\",\"STOT\":\"STOT\",\"SNUM\":\"SNUM\",\"CPN\":[{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"},{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"}]},{\"TKNB\":\"TKNB\",\"STOT\":\"STOT\",\"SNUM\":\"SNUM\",\"CPN\":[{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"},{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"}]}]}]}";
    stringstream msg_ss(msg_str);
    ptree msg;
    read_json(msg_ss, msg);
    string val = "";
    for(ptree::iterator msg_it = msg.begin(); msg_it != msg.end(); ++msg_it)
    {
        string msg_key = msg_it->first;
        if("TIF" == msg_key)
        {
            ptree tif_node = msg.get_child(msg_key);
            for(ptree::iterator tif_node_it = tif_node.begin(); tif_node_it != tif_node.end(); ++tif_node_it)
            {
                ptree tif = tif_node_it->second;
                for(ptree::iterator tif_it = tif.begin(); tif_it != tif.end(); ++tif_it)
                {
                    string tif_key = tif_it->first;
                    if("TKT" == tif_key)
                    {
                        ptree tkt_node = tif.get_child(tif_key);
                        for(ptree::iterator tkt_node_it = tkt_node.begin(); tkt_node_it != tkt_node.end(); ++tkt_node_it)
                        {
                            ptree tkt = tkt_node_it->second;
                            for(ptree::iterator tkt_it = tkt.begin(); tkt_it != tkt.end(); ++tkt_it)
                            {
                                string tkt_key = tkt_it->first;
                                if("CPN" == tkt_key)
                                {
                                    ptree cpn_node = tkt.get_child(tkt_key);
                                    for(ptree::iterator cpn_node_it = cpn_node.begin(); cpn_node_it != cpn_node.end(); ++cpn_node_it)
                                    {
                                        ptree cpn = cpn_node_it->second;
                                        for(ptree::iterator cpn_it = cpn.begin(); cpn_it != cpn.end(); ++cpn_it)
                                        {

                                            string cpn_key = cpn_it->first;
                                            val = cpn.get<string>(cpn_key);
                                            cout<< cpn_key <<":"<<val<<endl;
                                        }
                                    }
                                }
                                else
                                {

                                    val = tkt.get<string>(tkt_key);
                                    cout<<tkt_key<<":"<<val<<endl;
                                }
                            }
                        }
                    }
                    else
                    {

                        val = tif.get<string>(tif_key);
                        cout<< tif_key << ":" <<val <<endl;
                    }
                }
            }
        }
        else
        {

            val = msg.get<string>(msg_key);
            cout<< msg_key << ":" <<val<<endl;
        }
    }
}

int main()
{
    //    write_json_data_into_file();
    //    system("pause");
    //    read_json_data_from_file();
    //    system("pause");
    //    write_json_data_into_string();
    //    system("pause");
    //    read_json_data_from_string();
    cout<<"---------------------------"<<endl;
    testMsg();
    cout<<"---------------------------"<<endl;
    bianli_json();
    cout<<"---------------------------"<<endl;
    test();
    cout<<"---------------------------"<<endl;
    parseMsg();
    return 0;
}
