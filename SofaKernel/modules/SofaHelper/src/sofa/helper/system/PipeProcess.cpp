/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include "PipeProcess.h"

#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <WinNt.h>
#include <winsock.h>
#include <cstdio>
typedef int ssize_t;
typedef HANDLE fd_t;
typedef SOCKET socket_t;
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
typedef int fd_t;
typedef int socket_t;
#endif

#include <cstring>
#include <iostream>
#include <sstream>

#define BUFSIZE (64*1024-1)
#define STEPSIZE (1024)
//#define STEPSIZE BUFSIZE

#include <sofa/helper/logging/Messaging.h>

namespace sofa
{

namespace helper
{

namespace system
{

PipeProcess::PipeProcess()
{
}

PipeProcess::~PipeProcess()
{
}
/**   File as Stdin for windows does not work (yet)
  *   So the filename must be given into the args vector
  *   and argument filenameStdin is currently ignored
  */

bool PipeProcess::executeProcess(const std::string &command,  const std::vector<std::string> &args, const std::string &/*filenameStdin*/, std::string & outString, std::string & errorString)
{
    return false;
}

}
}
}

