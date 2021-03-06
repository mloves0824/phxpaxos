/*
Tencent is pleased to support the open source community by making 
PhxPaxos available.
Copyright (C) 2016 THL A29 Limited, a Tencent company. 
All rights reserved.

Licensed under the BSD 3-Clause License (the "License"); you may 
not use this file except in compliance with the License. You may 
obtain a copy of the License at

https://opensource.org/licenses/BSD-3-Clause

Unless required by applicable law or agreed to in writing, software 
distributed under the License is distributed on an "AS IS" basis, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
implied. See the License for the specific language governing 
permissions and limitations under the License.

See the AUTHORS file for names of contributors. 
*/

#pragma once

#include "phxpaxos/node.h"
#include "echo_sm.h"
#include <string>
#include <vector>
#include "phxpaxos_plugin/logger_google.h"

namespace phxecho
{


//PhxEchoServer其实对Node的封装，RunPaxos最终调用的是Node::RunNode，Echo最终调用的m_poPaxosNode->Propose
class PhxEchoServer
{
public:
    PhxEchoServer(const phxpaxos::NodeInfo & oMyNode, const phxpaxos::NodeInfoList & vecNodeList);
    ~PhxEchoServer();

    int RunPaxos();

    int Echo(const std::string & sEchoReqValue, std::string & sEchoRespValue);

private:
    int MakeLogStoragePath(std::string & sLogStoragePath);

private:
	//oMyNode标识本机的IP/PORT信息，vecNodeList标识多副本集群的所有机器信息
    phxpaxos::NodeInfo m_oMyNode;
    phxpaxos::NodeInfoList m_vecNodeList;

	//m_poPaxosNode代表了需要运行的PhxPaxos实例指针
    phxpaxos::Node * m_poPaxosNode;
	//状态机
    PhxEchoSM m_oEchoSM;
};
    
}


