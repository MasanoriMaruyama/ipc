/*
 * Copyright (c) 2021, Nippon Seiki Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include <ipc.h>
#include "ipc_internal.h"

int ipcCreateDomainName(IPC_USAGE_TYPE_E usageType, char *pOutName, int *pSize)
{
    int ret = -1;
    int len;
    char *ipcDomainPath; // from getenv
    char *domainName;

    IPC_E_CHECK(CHECK_VALID_USAGE(usageType), usageType, end);
    IPC_E_CHECK(pSize != NULL, 0, end);
    IPC_E_CHECK(pOutName != NULL, 0, end);

    domainName = g_ipcDomainInfoList[usageType].domainName;
    len = strlen(domainName);
    ipcDomainPath = getenv(IPC_ENV_DOMAIN_SOCKET_PATH);
    if (ipcDomainPath != NULL) {
        len += strlen(ipcDomainPath);
    }

    IPC_E_CHECK(*pSize > len, len, end);

    strcpy(pOutName, "");
    if (ipcDomainPath != NULL) {
        sprintf(pOutName, "%s/", ipcDomainPath);
    }
    strcat(pOutName, domainName);

    *pSize = strlen(pOutName) + 1;

    ret = 0;
end:
    return ret;
}

int ipcCreateUnixDomainAddr(const char *domainName, struct sockaddr_un *pOutUnixAddr, int *pOutLen)
{
    int ret = -1;

    IPC_E_CHECK(pOutLen != NULL, 0, end);
    IPC_E_CHECK(pOutUnixAddr != NULL, 0, end);

    pOutUnixAddr->sun_family = AF_UNIX;
    strcpy(pOutUnixAddr->sun_path, domainName);
    *pOutLen = sizeof(pOutUnixAddr->sun_family)+strlen(pOutUnixAddr->sun_path);

    ret = 0;
end:
    return ret;
}

