/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <assert.h>
#include "NFRedisTestModule.h"

NFRedisTestModule::NFRedisTestModule(NFIPluginManager* pluginManager)
{
	this->pPluginManager = pluginManager;
	this->testModule = true;
}

bool NFRedisTestModule::AfterInit()
{
	noSqlModule = pPluginManager->FindModule<NFINoSqlModule>();

	return true;
}

void NFRedisTestModule::TestCluster()
{
	std::string info;
	noSqlModule->INFO(info);
	std::vector<std::string> infoData;
	StringToVector(info, NFREDIS_CRLF, infoData);

	std::string clusterInfo;
	noSqlModule->CLUSTERINFO(clusterInfo);
	std::vector<std::string> clusterInfoData;
	StringToVector(clusterInfo, NFREDIS_CRLF, clusterInfoData);

	std::vector<std::string> masterNodes;
	noSqlModule->CLUSTERNODES(masterNodes);
	for (auto& node : masterNodes)
	{
		NF_TEST_SECTION("master node:" + node)
	}
}

void NFRedisTestModule::TestHash()
{
	int64_t nnn;
	NF_ASSERT("TestHash HINCRBY", noSqlModule->HINCRBY("HINCRBY12123ddd121wssdsdsdd", "121212", 13, nnn) == true);
	NF_ASSERT("TestHash nnn", nnn == 13);

	float fval;
	NF_ASSERT("TestHash HINCRBYFLOAT", noSqlModule->HINCRBYFLOAT("12123dd323ssss123ddd", "12444441212", 3.0f, fval) == true);

	NF_ASSERT("TestHash HSETNX", noSqlModule->HSETNX("12123ddd121wssdsdsdd", "12444441212", "121212") == true);
	NF_ASSERT("TestHash HSETNX", noSqlModule->HSETNX("12123ddd121wssdsdsdd", "12444441212", "121212") == false);

	std::string strKey = "TestHash::TestHash ";
	std::string strField1 = "TestField1";
	std::string strField2 = "TestField2";
	std::string strField3 = "TestField3";
	std::string strField4 = "TestField4";
	std::string strField5 = "TestField5";
	std::string strValue1 = "12TestHash231";
	std::string strValue2 = "12TestHash232222";
	std::string strValue3 = "12TestHash23r33333";
	std::string strValue4 = "12TestHash23r4444";
	std::string strValue5 = "12TestHash23r5555";

	string_vector fields;
	fields.push_back(strField1);
	fields.push_back(strField2);
	fields.push_back(strField3);
	fields.push_back(strField4);
	fields.push_back(strField5);

	string_vector values;
	values.push_back(strValue1);
	values.push_back(strValue2);
	values.push_back(strValue3);
	values.push_back(strValue4);
	values.push_back(strValue5);

	std::vector<string_pair> pair_values;
	pair_values.push_back(string_pair(fields[0], values[0]));
	pair_values.push_back(string_pair(fields[1], values[1]));
	pair_values.push_back(string_pair(fields[2], values[2]));
	pair_values.push_back(string_pair(fields[3], values[3]));
	pair_values.push_back(string_pair(fields[4], values[4]));


	NF_ASSERT("TestHash DEL", noSqlModule->DEL(strKey) == true);

	NF_ASSERT("TestHash HSET", noSqlModule->HSET(strKey, strField1, strValue1) == true);

	std::string strHGET;
	NF_ASSERT("TestHash HGET", noSqlModule->HGET(strKey, strField1, strHGET) == true);
	NF_ASSERT("TestHash strHGET == strValue1", strHGET == strValue1);

	NF_ASSERT("TestHash HEXISTS", noSqlModule->HEXISTS(strKey, strField1) == true);
	NF_ASSERT("TestHash HDEL", noSqlModule->HDEL(strKey, strField1) != 0);
	NF_ASSERT("TestHash HEXISTS", noSqlModule->HEXISTS(strKey, strField1) == false);
	NF_ASSERT("TestHash HMSET", noSqlModule->HMSET(strKey, pair_values) == true);
	
	int nHLEN;
	NF_ASSERT("TestHash HLEN", noSqlModule->HLEN(strKey, nHLEN) == true);
	NF_ASSERT("TestHash nHLEN == pair_values.size()", nHLEN == pair_values.size());

	string_vector getValue;
	NF_ASSERT("TestHash HMGET", noSqlModule->HMGET(strKey, fields, getValue) == true);
	NF_ASSERT("TestHash fields.size() == getValue.size()", fields.size() == getValue.size());
	for (int i = 0; i < values.size(); ++i)
	{
		NF_ASSERT("TestHash values[i] == getValue[i]", values[i] == getValue[i]);
	}

	std::vector<string_pair> get_pair_values;
	NF_ASSERT("TestHash HGETALL", noSqlModule->HGETALL(strKey, get_pair_values) == true);
	NF_ASSERT("TestHash get_pair_values.size() == fields.size()", get_pair_values.size() == fields.size());
	for (int i = 0; i < fields.size(); ++i)
	{
		NF_ASSERT("TestHash fields[i] == get_pair_values[i].first", fields[i] == get_pair_values[i].first);
		NF_ASSERT("TestHash values[i] == get_pair_values[i].second", values[i] == get_pair_values[i].second);
	}

	std::vector<std::string> get_keys;
	NF_ASSERT("TestHash HKEYS", noSqlModule->HKEYS(strKey, get_keys) == true);
	NF_ASSERT("TestHash get_keys.size() == fields.size()", get_keys.size() == fields.size());
	for (int i = 0; i < fields.size(); ++i)
	{
		NF_ASSERT("TestHash fields[i] == get_keys[i]", fields[i] == get_keys[i]);
	}

	std::vector<std::string> get_valuess;
	NF_ASSERT("TestHash HVALS", noSqlModule->HVALS(strKey, get_valuess) == true);
	NF_ASSERT("TestHash get_valuess.size() == values.size()", get_valuess.size() == values.size());
	for (int i = 0; i < values.size(); ++i)
	{
		NF_ASSERT("TestHash values[i] == get_valuess[i]", values[i] == get_valuess[i]);
	}

	for (int i = 0; i < fields.size(); ++i)
	{
		int nHSTRLEN;
		NF_ASSERT("TestHash HSTRLEN ", noSqlModule->HSTRLEN(strKey, fields[i], nHSTRLEN) == true);
		NF_ASSERT("TestHash nHSTRLEN == get_valuess[i].length()", nHSTRLEN == get_valuess[i].length());
	}
}

void NFRedisTestModule::TestKey()
{
	int64_t nnn;
	NF_ASSERT("TestKey INCRBY ", noSqlModule->INCRBY("INCRBY3ddd121wssdsdsdd", 13, nnn) == true);
	NF_ASSERT("TestKey nnn == 13", nnn == 13);

	float fval;
	NF_ASSERT("TestKey INCRBYFLOAT", noSqlModule->INCRBYFLOAT("12123dd323123ddd", 3.0f, fval) == true && std::abs(fval - 3.0f) < 0.01f);

	NF_ASSERT("TestKey SETNX", noSqlModule->SETNX("12123dd323123ddd", "121212") == false);
	NF_ASSERT("TestKey SETNX", noSqlModule->SETNX("124422dd1212", "121212") == true);


	std::string strKey = "NFRedisTestModule::TestKey";
	std::string strValue = "1232321123r34234";

	NF_ASSERT("TestKey SET", noSqlModule->SET(strKey, strValue) == true);

	std::string type;
	NF_ASSERT("TestKey TYPE", noSqlModule->TYPE(strKey, type) == true && type == "string");

	NF_ASSERT("TestKey DEL", noSqlModule->DEL(strKey) == true);
	NF_ASSERT("TestKey EXISTS", noSqlModule->EXISTS(strKey) == false);

	NF_ASSERT("TestKey SET", noSqlModule->SET(strKey, strValue) == true);
	NF_ASSERT("TestKey EXISTS", noSqlModule->EXISTS(strKey) == true);

	NF_ASSERT("TestKey EXPIRE", noSqlModule->EXPIRE(strKey, 2) == true);

	NFSLEEP(3000);
	std::string strGET;
	NF_ASSERT("TestKey GET", noSqlModule->GET(strKey, strGET) == false);
	NF_ASSERT("TestKey strGET == \"\"", strGET == "");

	//pRedisResult = noSqlModule->EXPIREAT(strKey, const int64_t unixTime);
	NF_ASSERT("TestKey PERSIST", noSqlModule->PERSIST(strKey) == false);

	int ttl;
	NF_ASSERT("TestKey TTL", noSqlModule->TTL(strKey, ttl) == true && ttl == -2);
	NF_ASSERT("TestKey TYPE", noSqlModule->TYPE(strKey, type) ==true && type == "none");

}

void NFRedisTestModule::TestList()
{
	/*
	NF_SHARE_PTR<NFRedisResult> RPUSHX(const std::string& key, const std::string& value);

	NF_SHARE_PTR<NFRedisResult> LSET(const std::string& key, const int index, const std::string& value);

	*/

	std::string strKey = "TestList::";
	std::vector<std::string> strList;
	strList.push_back("123232111111134");
	strList.push_back("1232321222222");
	strList.push_back("123232113333");
	strList.push_back("123444444");


	NF_ASSERT("TestList LSET", noSqlModule->LSET("12122121", 1, "12212") == false);
	int length = 0;
	NF_ASSERT("TestList LPUSH", noSqlModule->LPUSH("12122121", "12212", length) == true && length != 0);
	std::string lpop;
	NF_ASSERT("TestList LPOP", noSqlModule->LPOP("12122121", lpop) == true);
	NF_ASSERT("TestList \"12212\" == lpop", "12212" == lpop);

	NF_ASSERT("TestList HLDELEN", noSqlModule->DEL(strKey) == true);

	for (int i = 0; i < strList.size(); ++i)
	{
		NF_ASSERT("TestList RPUSH", noSqlModule->RPUSH(strKey, strList[i], length) == true && length == (i + 1));
	}

	for (int i = 0; i < strList.size(); ++i)
	{
		std::string strLINDEX;
		NF_ASSERT("TestList LINDEX", noSqlModule->LINDEX(strKey, i, strLINDEX) == true);
		NF_ASSERT("TestList strLINDEX == strList[i]", strLINDEX == strList[i]);
	}

	int nLLEN = 0;
	NF_ASSERT("TestList LLEN", noSqlModule->LLEN(strKey, nLLEN) == true);
	NF_ASSERT("TestList nLLEN == strList.size()", nLLEN == strList.size());

	for (int i = strList.size() - 1; i >= 0; --i)
	{
		std::string strRPOP;
		NF_ASSERT("TestList HRPOPLEN", noSqlModule->RPOP(strKey, strRPOP) == true);
		NF_ASSERT("TestList strRPOP == strList[i]", strRPOP == strList[i]);
	}

	NF_ASSERT("TestList LLEN", noSqlModule->LLEN(strKey, nLLEN) == true);
	NF_ASSERT("TestList nLLEN == 0", nLLEN == 0);
	//////
	for (int i = strList.size() - 1; i >= 0; --i)
	{
		int length = 0;
		NF_ASSERT("TestList LPUSH ", noSqlModule->LPUSH(strKey, strList[i], length) == true && length == (strList.size()-i));
	}

	for (int i = 0; i < strList.size(); ++i)
	{
		std::string strLINDEX;
		NF_ASSERT("TestList LINDEX", noSqlModule->LINDEX(strKey, (int)i, strLINDEX) == true);
		NF_ASSERT("TestList strLINDEX == strList[i]", strLINDEX == strList[i]);
	}

	NF_ASSERT("TestList LLEN", noSqlModule->LLEN(strKey, nLLEN) == true);
	NF_ASSERT("TestList , nLLEN == strList.size()", nLLEN == strList.size());

	for (int i = 0; i < strList.size(); ++i)
	{
		std::string strLPOP;
		NF_ASSERT("TestList LPOP", noSqlModule->LPOP(strKey, strLPOP) == true);
		NF_ASSERT("TestList strLPOP == strList[i]", strLPOP == strList[i]);
	}

	NF_ASSERT("TestList LLEN", noSqlModule->LLEN(strKey, nLLEN) == true);
	NF_ASSERT("TestList nLLEN == 0", nLLEN == 0);
	/////
	NF_ASSERT("TestList LPUSH", noSqlModule->LPUSH(strKey, strKey, length) == true && length == 1);

	NF_ASSERT("TestList LLEN", noSqlModule->LLEN(strKey, nLLEN) == true);
	NF_ASSERT("TestList nLLEN == 1", nLLEN == 1);

	for (int i = 0; i < strList.size(); ++i)
	{
		NF_ASSERT("TestList LPUSHX", noSqlModule->LPUSHX(strKey, strList[i], length) == true && length ==  (i+2));
	}

	string_vector values;
	NF_ASSERT("TestList LRANGE", noSqlModule->LRANGE(strKey, 0, (int)strList.size(), values) == true);
	NF_ASSERT("TestList values.size() == strList.size() + 1", values.size() == strList.size() + 1);
}

void NFRedisTestModule::TestSet()
{
	struct A
	{
		int a = 100;
		short b = 200;
	};
	A a;

	std::string strKey = "TestSetKey1:";
	std::string strBinary((char*)&a, sizeof(a));
	int count = 0;
	noSqlModule->SADD(strKey, strBinary, count);

	string_vector members;
	noSqlModule->SMEMBERS(strKey, members);
	if (members.size() > 0)
	{
		A* pa = (A*)members[0].c_str();
		NF_ASSERT("TestSet HLEN", pa->a == 100 && pa->b == 200);
	}
	else
	{
		NF_ASSERT("TestSet HLEN", false);
	}
}

void NFRedisTestModule::TestSort()
{

}

void NFRedisTestModule::TestString()
{
	std::string strKey11 = "TestString1112121";
	std::string strValu11e = "111";

	int64_t nValueDECR;
	NF_ASSERT("TestString DECR", noSqlModule->DECR(strKey11, nValueDECR) == true);
	NF_ASSERT("TestString SET", noSqlModule->SET(strKey11, strValu11e) == true);
	NF_ASSERT("TestString DECR", noSqlModule->DECR(strKey11, nValueDECR) == true);
	NF_ASSERT("TestString nValueDECR == 110", nValueDECR == 110);
	NF_ASSERT("TestString DECRBY", noSqlModule->DECRBY(strKey11, 10, nValueDECR) == true);
	NF_ASSERT("TestString nValueDECR == 100", nValueDECR == 100);
	std::string oldGETSET;
	NF_ASSERT("TestString GETSET", noSqlModule->GETSET(strKey11, "200", oldGETSET) == true);
	NF_ASSERT("TestString oldGETSET == \"100\"", oldGETSET == "100");

	NF_ASSERT("TestString INCR", noSqlModule->INCR(strKey11, nValueDECR) == true);
	NF_ASSERT("TestString nValueDECR == 201", nValueDECR == 201);

	NF_ASSERT("TestString INCRBY", noSqlModule->INCRBY(strKey11, 100, nValueDECR) == true);
	NF_ASSERT("TestString nValueDECR == 301", nValueDECR == 301);

	std::string strKey = "TestString";
	std::string strValue = "1232TestString234";

	NF_ASSERT("TestString SET ", noSqlModule->SET(strKey, strValue) == true);

	std::string strGET;
	NF_ASSERT("TestString GET", noSqlModule->GET(strKey, strGET) == true);
	NF_ASSERT("TestString strGET == strValue", strGET == strValue);

	int nAPPEND = 0;
	NF_ASSERT("TestString APPEND", noSqlModule->APPEND(strKey, strValue, nAPPEND) == true);
	NF_ASSERT("TestString nAPPEND == strValue.length() * 2 ", nAPPEND == strValue.length() * 2);

	int nSTRLEN = 0;
	NF_ASSERT("TestString STRLEN", noSqlModule->STRLEN(strKey, nSTRLEN) == true);
	NF_ASSERT("TestString nSTRLEN == strValue.length() * 2", nSTRLEN == strValue.length() * 2);
	NF_ASSERT("TestString STRLEN", noSqlModule->STRLEN("321321", nSTRLEN) == true);
	NF_ASSERT("TestString STRLEN", nSTRLEN == 0);

    std::cout << "test cmd:" << std::endl;

    std::vector<string_pair> vstring_pair;
    std::vector<std::string> vstringListKey;

    for (int i = 0; i < 5; ++i)
    {
        string_pair vPair;

        std::stringstream streamFirst;
        std::stringstream streamSecond;
        streamFirst << "MSET_KEY" << i;
        streamSecond << "MSET_Value" << i;
        streamFirst >> vPair.first;
        streamSecond >> vPair.second;

        vstring_pair.push_back(vPair);

        vstringListKey.push_back(vPair.first);

    }

    //noSqlModule->MSET(vstring_pair);

	std::vector<std::string> vstringListValue;
    //noSqlModule->MGET(vstringListKey, vstringListValue);
	//NF_ASSERT("TestString  vstringListKey.size() == vstringListValue.size()", vstringListKey.size() == vstringListValue.size());

	for (int i = 0; i < vstringListKey.size(); ++i)
	{
		std::string strGET;
		//NF_ASSERT("TestString GET", noSqlModule->GET(vstringListKey[i], strGET) == true);
		//NF_ASSERT("TestString strGET == vstringListValue[i]", strGET == vstringListValue[i]);
	}

}

bool NFRedisTestModule::Execute()
{
	if (noSqlModule->Enable())
	{
		noSqlModule->FLUSHALL();

		TestCluster();

		TestKey();
		TestString();
		TestList();
		TestHash();
		TestSet();
		TestSort();

		PerformanceTest();
	}

    return false;
}

void NFRedisTestModule::PerformanceTest()
{
	std::string strKey = "PerformanceTest";

	int64_t startTime;
	int64_t endTime;

	//1 million
	int totalCount = 200002;

	startTime = NFGetTimeMS();
	NF_TEST_SECTION("start PerformanceTest  write:" + std::to_string(startTime) + ", test count:" + std::to_string(totalCount))
	for (int i = 0; i < totalCount; ++i)
	{
		noSqlModule->HSET(strKey + std::to_string(i), std::to_string(i), std::to_string(i));
		//NF_REQUIRE_EQUAL("", noSqlModule->HSET(strKey, std::to_string(i), std::to_string(i)),true);
	}
	endTime = NFGetTimeMS();
	NF_TEST_SECTION("end PerformanceTest write:" + std::to_string(endTime) + ", delta:" + std::to_string(endTime - startTime) + ", tps:" + std::to_string(totalCount * 1000 / (endTime - startTime)))


	startTime = NFGetTimeMS();
	NF_TEST_SECTION("start PerformanceTest read:" + std::to_string(startTime) + ", test  count:" + std::to_string(totalCount))
	for (int i = 0; i < totalCount; ++i)
	{
		std::string value;
		noSqlModule->HGET(strKey + std::to_string(i), std::to_string(i), value);
		//NF_REQUIRE_EQUAL("", value,std::to_string(i));
	}
	endTime = NFGetTimeMS();
	NF_TEST_SECTION("end PerformanceTest read:" + std::to_string(endTime) + ", delta:" + std::to_string(endTime - startTime) + ", qps:" + std::to_string(totalCount * 1000 / (endTime - startTime)))
}