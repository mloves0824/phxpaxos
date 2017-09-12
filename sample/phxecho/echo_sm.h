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

#include "phxpaxos/sm.h"
#include "phxpaxos/options.h"
#include <stdio.h>
#include <unistd.h>

namespace phxecho
{

class PhxEchoSMCtx
{
public:
	//通过iExecuteRet可以获得Execute的执行情况，通过sEchoRespValue可以获得Execute带入的sEchoReqValue
    int iExecuteRet;
    std::string sEchoRespValue;

    PhxEchoSMCtx()
    {
        iExecuteRet = -1;
    }
};

//定义一个状态机叫PhxEchoSM，这个类继承自StateMachine类
class PhxEchoSM : public phxpaxos::StateMachine
{
public:
    PhxEchoSM();

	//Execute为状态机状态转移函数，输入为sPaxosValue， PhxPaxos保证多台机器都会执行相同系列的Execute(sPaxosValue)， 从而获得强一致性
    bool Execute(const int iGroupIdx, const uint64_t llInstanceID, 
            const std::string & sPaxosValue, phxpaxos::SMCtx * poSMCtx);

	//因为一个PhxPaxos可以同时挂载多个状态机，所以需要SMID()这个函数返回这个状态机的唯一标识ID

    const int SMID() const { return 1; }
};
    
}
