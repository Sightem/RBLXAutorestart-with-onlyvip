#include "Functions.h"

void Functions::Log(const std::string& text, bool error)
{
    std::cout << (error ? " [ error ] " + text : " [ autorestart ] " + text) << std::endl;
}

void Functions::Debounce(int t, int c)
{
    auto start = std::chrono::steady_clock::now(); 
    // need to trunicate and fix float percision
    switch (c) {
    case 0: { while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() <= t) { Sleep(10); } break; }
    case 1: { while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() <= t) { Sleep(10); } break; }     
    case 2: { while (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() <= t) { Sleep(10); } break; }    
    default: { break; }                                                                                                                        
    }
}

double Functions::GetRam()
{
    MEMORYSTATUSEX MemoryStatusEx;
    MemoryStatusEx.dwLength = sizeof(MemoryStatusEx);
    GlobalMemoryStatusEx(&MemoryStatusEx);
    return (floor(MemoryStatusEx.ullTotalPhys / (1024 * 1024 * 1024) + 1));
}

double Functions::GetThreads()
{
    return (std::thread::hardware_concurrency());
}

int Functions::CountInstances()
{
    int count = 0;
    WTS_PROCESS_INFO* pWPIs = NULL;
    DWORD dwProcCount = 0;
    if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, NULL, 1, &pWPIs, &dwProcCount)) {
        for (DWORD i = 0; i < dwProcCount; i++)
        {
            std::string name = std::string(pWPIs[i].pProcessName);
            if (name.find("Roblox") != std::string::npos) {
                count++;
            }
        }
    }
    if (pWPIs) { WTSFreeMemory(pWPIs); pWPIs = NULL; }
    return count;
}

bool Functions::IsProcessRunning(const TCHAR* const executableName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry)) {
        CloseHandle(snapshot);
        return false;
    }

    do {
        if (!_tcsicmp(entry.szExeFile, executableName)) {
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;
}

void Functions::KillProccess(const char* pname)
{
    if (Functions::IsProcessRunning(pname)) {
        HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);
        BOOL hRes = Process32First(hSnapShot, &pEntry);
        while (hRes) {
            if (strcmp(pEntry.szExeFile, pname) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                    (DWORD)pEntry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 9);
                    CloseHandle(hProcess);
                }
            }
            hRes = Process32Next(hSnapShot, &pEntry);
        }
        CloseHandle(hSnapShot);
    }
}

void Functions::RunBrowsers()
{
    system("cls");
    Roblox Roblox;
    Functions Functions;
    for (int i = 0; i < Functions::Browsers.size(); i++)
    {
        Functions.Log("Running " + Functions::Browsers[i]);
        std::string cmd = "start " + Functions::Browsers[i] + " " + Functions::VIP;
        system(cmd.c_str());
        Functions::Debounce(Functions::SafeMode ? 45 : 15, 1);
        Functions::KillBrowsers();
        while (Functions::IsProcessRunning(Functions::Browsers[i].c_str())) 
        {
            Functions::Debounce(10, 0);
        }
    }
    Functions::Debounce(100, 0);
}

void Functions::KillBrowsers()
{
    system("cls");
    Functions Functions;

    for (int i = 0; i < Functions::Browsers.size(); i++)
    {
        Functions.Log("Killing " + Functions::Browsers[i]);
        if (Functions::FindString(Functions::Browsers[i].c_str(), "\\"))
        {
            size_t handle = 0;
            while ((handle = Functions::FindString(Functions::Browsers[i].c_str(), "\\")))
            {
                Functions::Browsers[i].erase(0, handle + 2);
            }

            Functions::KillProccess(Functions::Browsers[i].c_str());
        }
        else 
        {
            Functions::KillProccess(Functions::Browsers[i].c_str());
            while (Functions::IsProcessRunning(Functions::Browsers[i].c_str())) { Functions::Debounce(1, 1); }
        }
    }
}

bool Functions::HasEnoughRam()
{
    return (Functions::GetRam() >= (8 - .75) ? true : false);
}

bool Functions::HasEnoughThreads()
{
    return (Functions::GetThreads() >= 3 ? true : false);
}

bool Functions::FindString(const char* a, const char* b)
{
    return (std::string(a).find(std::string(b)) != std::string::npos) ? true : false;
}

bool Functions::IsNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void Roblox::Unlock()
{
    //-- hook roblox's singleton mutex and return its binitial, then hook binitial and force it null
    CreateMutex(NULL, TRUE, "ROBLOX_singletonMutex");
}

void Functions::_usleep(int microseconds)
{
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

void Functions::wait(int ms)
{
    std::chrono::milliseconds t(ms);
    std::this_thread::sleep_for(t);
}

bool Roblox::FindError()
{
    char buff[FILENAME_MAX];
    _getcwd(buff, FILENAME_MAX);
    std::string program_path(buff);
    std::string error_file = program_path.append("\\error.ini");
    std::ifstream err(error_file);
    bool error_found = (bool)err;
    if (error_found) 
    {
        if (err.is_open()) 
        {
            std::string line;
            std::string content;
            int l = 0;
            while (std::getline(err, line)) 
            {
                l++;
                switch (l) 
                {
                case 1:
                    content = line;
                    break;

                default:
                    break;
                }
            }
            if (content == "1") 
            {
                return true;
            }
        }
    }
    return false;
}

bool Roblox::OnlyVip()
{
    char buff[FILENAME_MAX];
    _getcwd(buff, FILENAME_MAX);
    std::string program_path(buff);
    std::string vip_file = program_path.append("\\onlyvip.ini");
    std::ifstream vip(vip_file);
    bool vipisyes = (bool)vip;
    if (vipisyes)
    {
        if (vip.is_open())
        {
            std::string line;
            std::string content;
            int l = 0;
            while (std::getline(vip, line))
            {
                l++;
                switch (l)
                {
                case 1:
                    content = line;
                    break;

                default:
                    break;
                }
            }
            if (content == "1")
            {
                return true;
            }
        }
    }
    return false;
}

bool Roblox::FindRoblox() {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry)) {
        CloseHandle(snapshot);
        return false;
    }

    do {
        if (!_tcsicmp(entry.szExeFile, "RobloxPlayerBeta.exe")) {
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;
}

void Roblox::Kill()
{
    system("cls");
    Functions Functions;
    Functions.Log("Killing Roblox");
    bool found = Roblox::FindRoblox() ? true : false;
    if (found) {
        HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);
        BOOL hRes = Process32First(hSnapShot, &pEntry);
        while (hRes) {
            if (strcmp(pEntry.szExeFile, "RobloxPlayerBeta.exe") == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                    (DWORD)pEntry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 9);
                    CloseHandle(hProcess);
                }
            }
            hRes = Process32Next(hSnapShot, &pEntry);
        }
        CloseHandle(hSnapShot);
    }
}

void Functions::LockWindow()
{
    SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void Roblox::Minimize(const char* pname)
{
    if (FindWindow(NULL, pname))
    {
        Functions Functions;
        int count = 0;
        WTS_PROCESS_INFO* pWPIs = NULL;
        DWORD dwProcCount = 0;
        if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, NULL, 1, &pWPIs, &dwProcCount))
        {
            for (DWORD i = 0; i < dwProcCount; i++)
            {
                std::string name = std::string(pWPIs[i].pProcessName);
                if (Functions.FindString(name.c_str(), pname))
                {
                    count++;
                }
            }
        }
        if (pWPIs) { WTSFreeMemory(pWPIs); pWPIs = NULL; }
        for (int i = 0; i < count; i++)
        {
            ShowWindow(FindWindow(NULL, pname), SW_FORCEMINIMIZE);
        }
    }
}

double Functions::WindowsDeployment()
{
    double ret = 0.0;
    NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
    OSVERSIONINFOEXW osInfo;

    *(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

    if (NULL != RtlGetVersion)
    {
        osInfo.dwOSVersionInfoSize = sizeof(osInfo);
        RtlGetVersion(&osInfo);
        ret = (double)osInfo.dwMajorVersion;
    }
    return ret;
}

void Functions::CreateConfig()
{
    Roblox Roblox;
    std::ifstream file("config.ini");
    if (!(bool)file)
    {
        std::ofstream CreateConfig("config.ini");
        CreateConfig << "vip" << std::endl;
        CreateConfig << "time" << std::endl;
        CreateConfig << "browser" << std::endl;
        CreateConfig.close();
        exit(0);
    }
    else
    {
        if (file.is_open())
        {
            std::string text;
            int line = 0;
            while (std::getline(file, text))
            {
                line++;
                switch (line)
                {
                    case 1:
                    {
                        if (Functions::FindString(text.c_str(), "2414851778"))
                        {
                            Functions::VIP = text;
                        }
                        else
                        {
                            Functions::Log("VIP Invalid. Please use a dungeon quest VIP server.", true);
                            Functions::Debounce(5, 1);
                            exit(001);
                        }
                        break;
                    }

                    case 2:
                    {
                        if (Functions::IsNumber(text))
                        {
                            std::stoi(text) <= 0 || std::stoi(text) >= 999 ? Functions::RestartTime = 15 : Functions::RestartTime = std::stoi(text);
                        }
                        else
                        {
                            Functions::Log("The time you entered is not a number.", true);
                            Functions::Debounce(5, 1);
                            exit(002);
                        }
                        break;
                    }

                    default:
                    {
                        if (Functions::FindString(text.c_str(), ".exe"))
                        {
                           Functions::Browsers.push_back(text);
                        }
                        else
                        {
                            Functions::Log(text + " is invalid. It should look like this: " + text + ".exe", true);
                            Debounce(5, 1);
                            exit(003);
                        }
                        break;
                    }
                }
            }
            Functions::Log("Setup config");
        }
    }
}