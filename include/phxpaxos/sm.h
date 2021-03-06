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

#include <string>
#include <vector>
#include <typeinfo>
#include <stdint.h>
#include <inttypes.h>

namespace phxpaxos
{

//SMCtx类型参数作为一个上下文，由写入者提供，并由PhxPaxos带到Execute函数，最终传递给用户使用。
class SMCtx
{
public:
    SMCtx();
    SMCtx(const int iSMID, void * pCtx);

	//状态机唯一标识
    int m_iSMID;
	//记录了用户自定义的上下文数据的所在地址
    void * m_pCtx;
};

//////////////////////////////////////////////

class CheckpointFileInfo
{
public:
    std::string m_sFilePath;
    size_t m_llFileSize;
};

typedef std::vector<CheckpointFileInfo> CheckpointFileInfoList;

/////////////////////////////////////////////

const uint64_t NoCheckpoint = (uint64_t)-1; 

class StateMachine
{
public:
    virtual ~StateMachine() {}

    //Return true means execute success. 
    //This 'success' means this execute don't need to retry.
    //Sometimes you will have some logical failure in your execute logic, 
    //and this failure will definite occur on all node, that means this failure is acceptable, 
    //for this case, return true is the best choice.
    //Some system failure will let different node's execute result inconsistent,
    //for this case, you must return false to retry this execute to avoid this system failure. 
    virtual bool Execute(const int iGroupIdx, const uint64_t llInstanceID, 
            const std::string & sPaxosValue, SMCtx * poSMCtx) = 0;

    //Different state machine return different SMID().
    virtual const int SMID() const = 0;

    virtual bool ExecuteForCheckpoint(const int iGroupIdx, const uint64_t llInstanceID, 
            const std::string & sPaxosValue);

    //Only need to implement this function while you have checkpoint.
    //Return your checkpoint's max executed instanceid.
    //Notice PhxPaxos will call this function very frequently.
    virtual const uint64_t GetCheckpointInstanceID(const int iGroupIdx) const;

    //After called this function, the vecFileList that GetCheckpointState return's, can't be delelted, moved and modifyed.
    virtual int LockCheckpointState();
    
    //sDirpath is checkpoint data root dir path.
    //vecFileList is the relative path of the sDirPath.
    virtual int GetCheckpointState(const int iGroupIdx, std::string & sDirPath, 
            std::vector<std::string> & vecFileList); 

    virtual void UnLockCheckpointState();
    
    //Checkpoint file was on dir(sCheckpointTmpFileDirPath).
    //vecFileList is all the file in dir(sCheckpointTmpFileDirPath).
    //vecFileList filepath is absolute path.
    //After called this fuction, paxoslib will kill the processor. 
    //State machine need to understand this when restart.
    virtual int LoadCheckpointState(const int iGroupIdx, const std::string & sCheckpointTmpFileDirPath,
            const std::vector<std::string> & vecFileList, const uint64_t llCheckpointInstanceID);
};
    
}
