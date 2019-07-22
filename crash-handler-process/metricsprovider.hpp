/******************************************************************************
    Copyright (C) 2016-2019 by Streamlabs (General Workings Inc)
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <thread>
#include <mutex>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <map>

class MetricsProvider
{
    const wchar_t*   CrashMetricsFilename = L"backend-metrics.doc";
    const static int StringSize = 64;

    enum class MessageType
    {
        Pid,
        Tag, 
        Status, 
        Blame, 
        Shutdown
    };

    struct MetricsMessage
    {
        MessageType type;
        char param1[StringSize];
        char param2[StringSize];
    };

    public:

    MetricsProvider();
    ~MetricsProvider();

    bool Initialize(std::wstring name, std::string version, bool isDevEnv);
    void Shutdown();
    bool ConnectToClient();
    void StartPollingEvent();

    void KillPendingIO();

    bool ServerIsActive();
    bool ServerExitedSuccessfully();

    void BlameFrontend();
    void BlameServer();

    private:

    void InitializeMetricsFile();
    void MetricsFileSetStatus(std::string status);
    void MetricsFileClose();
    void SendMetricsReport(std::string status);
    std::string GetMetricsFileStatus();

    private:

    HANDLE        m_Pipe;
    HANDLE        m_ClientConnectionThread = nullptr;
    bool		  m_PipeActive = false;
    bool          m_StopPolling = false;
    bool          m_ServerExitedSuccessfully = false;
    bool          m_ClientBlameActive = false;
    bool          m_IsDevEnv = false;
    std::thread   m_PollingThread;
    std::ofstream m_MetricsFile;
    DWORD         m_ServerPid = 0;
    std::wstring  m_MetricsFilePath;
    std::string   m_LastStatus;
    std::map<std::string, std::string> m_ReportTags;
};