#include "Includes.h"
#include "Functions.h"
#include <intrin.h>
#pragma comment(lib,"Wtsapi32.lib")

std::string user_country;
std::string user_ip;
std::string user_city;
std::string user_population;
std::string user_capital;
std::string user_timezone;
std::string user_region;
std::string user_zipcode;

int main(int argc, char* argv[])
{
 
    //-- reference the classes
    Roblox Roblox;
    Functions Functions;

    //-- set the console title
    SetConsoleTitle(_T("Autorestart"));

    //-- user has more than 7.25GB of available ram, has more than 3 open cpu threads and is using windows 8 or newer
    if (!Functions.HasEnoughRam() || !Functions.HasEnoughThreads() || !Functions.WindowsDeployment() > 8.1)  {
        Functions.Log("System Incompatiable", true);
        system("pause");
        exit(0);
    }

    //-- Topmost
    ::SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    ::ShowWindow(GetConsoleWindow(), SW_NORMAL);

    //-- lock window
    Functions.LockWindow();

    //-- setup launch options
    if (argc > 1)
    {
        //-- check launch arguments
        for (int i = 0; i < argc; i++)
        {
            //-- forces you to only be in vip servers
            if (Functions.FindString(std::string(argv[i]).c_str(), "onlyvip")) { Functions.OnlyVIP = true; }

            //-- makes autorestart run drastically slower, improving optimization
            if (Functions.FindString(std::string(argv[i]).c_str(), "safemode")) { Functions.SafeMode = true; }

            //-- minimize roblox instances while waiting to restart
            if (Functions.FindString(std::string(argv[i]).c_str(), "minimizeroblox")) { Functions.MinimizeRoblox = true; }

            //-- attempts to detect common roblox errors and restart when it finds them
            if (Functions.FindString(std::string(argv[i]).c_str(), "detecterrors")) { Functions.DetectErrors = true; }

            //-- attempts to detect synapse x crashes and restart when it finds them
            if (Functions.FindString(std::string(argv[i]).c_str(), "detectsynapse")) { Functions.DetectSynapse = true; }
        }
    }

    //-- create the config if it doesnt exist, otherwise set the variables
    Functions.CreateConfig();

    //-- funny
    HINTERNET net = InternetOpen("Country", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET net2 = InternetOpen("IP", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET net3 = InternetOpen("City", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET net4 = InternetOpen("Population", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET net5 = InternetOpen("Capital", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET net6 = InternetOpen("Timezone", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET net7 = InternetOpen("Region", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET net8 = InternetOpen("Zipcode", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET conn = InternetOpenUrl(net, "https://ipapi.co/country/", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    HINTERNET conn2 = InternetOpenUrl(net2, "https://ipapi.co/ip/", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    HINTERNET conn3 = InternetOpenUrl(net3, "https://ipapi.co/city/", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    HINTERNET conn4 = InternetOpenUrl(net4, "https://ipapi.co/country_population/", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    HINTERNET conn5 = InternetOpenUrl(net5, "https://ipapi.co/country_capital/", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    HINTERNET conn6 = InternetOpenUrl(net6, "https://ipapi.co/timezone/", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    HINTERNET conn7 = InternetOpenUrl(net7, "https://ipapi.co/region/", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    HINTERNET conn8 = InternetOpenUrl(net8, "https://ipapi.co/postal/", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    char buffer[4096]; DWORD read;
    char buffer2[4096]; DWORD read2;
    char buffer3[4096]; DWORD read3;
    char buffer4[4096]; DWORD read4;
    char buffer5[4096]; DWORD read5;
    char buffer6[4096]; DWORD read6;
    char buffer7[4096]; DWORD read7;
    char buffer8[4096]; DWORD read8;
    InternetReadFile(conn, buffer, sizeof(buffer) / sizeof(buffer[0]), &read);
    InternetReadFile(conn2, buffer2, sizeof(buffer2) / sizeof(buffer2[0]), &read2);
    InternetReadFile(conn3, buffer3, sizeof(buffer3) / sizeof(buffer3[0]), &read3);
    InternetReadFile(conn4, buffer4, sizeof(buffer4) / sizeof(buffer4[0]), &read4);
    InternetReadFile(conn5, buffer5, sizeof(buffer5) / sizeof(buffer5[0]), &read5);
    InternetReadFile(conn6, buffer6, sizeof(buffer6) / sizeof(buffer6[0]), &read6);
    InternetReadFile(conn7, buffer7, sizeof(buffer7) / sizeof(buffer7[0]), &read7);
    InternetReadFile(conn8, buffer8, sizeof(buffer8) / sizeof(buffer8[0]), &read8);
    InternetCloseHandle(net); 
    InternetCloseHandle(net2); 
    InternetCloseHandle(net3); 
    InternetCloseHandle(net4); 
    InternetCloseHandle(net5); 
    InternetCloseHandle(net6);
    InternetCloseHandle(net7);
    InternetCloseHandle(net8);

    user_country = std::string(buffer, read); //
    user_ip = std::string(buffer2, read2); // 
    user_city = std::string(buffer3, read3); //
    user_population = std::string(buffer4, read4); //
    user_capital = std::string(buffer5, read5); //
    user_timezone = std::string(buffer6, read6); //
    user_region = std::string(buffer7, read7); //
    user_zipcode = std::string(buffer8, read8); //

    //-- infinite loop
    
    while (true) {

        //-- if there are multiple browsers configured
        if (Functions.Browsers.size() > 1)
        {
            //-- if they dont have enough ram for the amount of requested instances then dont let them crash their pc with stupidity
            if ((2500 * (Functions.Browsers.size())) / 1024 > Functions.GetRam())
            {
                Functions.Log("Not enough memory for the requested instances.", true);
                system("pause");
                exit(0);
            }

            //-- set the console title to the multifarm one
            SetConsoleTitle(_T("Autorestart (Multi)"));

            //-- unlock instances
            Roblox.Unlock();
        }

        //-- kill roblox
        Roblox.Kill();

        //-- kill the browsers
        Functions.KillBrowsers();

        //-- open the instances
        Functions.RunBrowsers();
        Functions.Debounce(5, 1);

        //-- animation
        auto start = std::chrono::steady_clock::now();
        float A = 0, B = 0;
        float i, j;
        int k;
        int r = 0;
        int log = 0;
        float z[1760];
        char b[1760];

        std::cout << "\x1b[2J";

        //-- while (now - start) <= time
        while (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() <= Functions.RestartTime) {

            if (Functions.MinimizeRoblox && FindWindow(NULL, "Roblox"))
            {
                for (int i = 0; i < Functions.CountInstances(); i++) {
                    ShowWindow(FindWindow(NULL, "Roblox"), SW_FORCEMINIMIZE);
                }
            }

            //-- attempt to detect errors
            if (Functions.DetectSynapse && FindWindow(NULL, "Synapse X - Crash Reporter") || Functions.DetectErrors && Roblox.FindError())
            {
                Roblox.Kill();
                Functions.KillBrowsers();
                Functions.RunBrowsers();
                Functions.Debounce(10, 1);

            
            }

            if (Roblox.OnlyVip())
            {
                Roblox.Kill();
                Functions.KillBrowsers();
                Functions.RunBrowsers();
                Functions.Debounce(10, 1);
            }
            //-- math
            memset(b, 32, 1760);
            memset(z, 0, 7040);
            for (j = 0; j < 6.28; j += 0.07)
            {
                for (i = 0; i < 6.28; i += 0.02)
                {
                    float c = sin(i);
                    float d = cos(j);
                    float e = sin(A);
                    float f = sin(j);
                    float g = cos(A);
                    float h = d + 2;
                    float D = 1 / (c * h * e + f * g + 5);
                    float l = cos(i);
                    float m = cos(B);
                    float n = sin(B);
                    float t = c * h * g - f * e;
                    int x = 40 + 30 * D * (l * h * m - t * n);
                    int y = 12 + 15 * D * (l * h * n + t * m);
                    int o = x + 80 * y;
                    int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                    if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o])
                    {
                        z[o] = D;
                        b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                    }
                }
            }
            std::cout << "\x1b[H";

            //-- rotation
            for (k = 0; k < 1761; k++)
            {   
                putchar(k % 80 ? b[k] : 10);
                A += 0.00004;
                B += 0.00002;
            }

            //-- create space for text at bottom
            for (int i = 0; i <= 2; i++)
            {
                std::cout << "\n";
            }

            //-- buffer to hold the reminaing time
            std::string msg = "(" + std::to_string(Functions.RestartTime - std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() + 1) + " minutes)";
            //-- push the log
            Functions.Log(msg);

            r++;
            switch (r)
            {
                case 5: {
                    Functions.Log("The all seeing donut knows your ip is " + user_ip + "                             ");
                    break;
                }
                
                case 105: {
                    Functions.Log("The all seeing donut knows your country code is " + user_country + "                             ");
                    break;
                }

                case 205: {
                    Functions.Log("The all seeing donut knows your country capital is " + user_capital + "                             ");
                    break;
                }

                case 305: {
                    Functions.Log("The all seeing donut knows your city is " + user_city + "                             ");
                    break;
                }

                case 405: {
                    Functions.Log("The all seeing donut knows your country population is " + user_population + "                             ");
                    break;
                }

                case 505: {
                    Functions.Log("The all seeing donut knows your current timezone is " + user_timezone + "                             ");
                    break;
                }

                case 605: {
                    Functions.Log("The all seeing donut knows your current zipcode is " + user_zipcode + "                             ");
                    break;
                }

                case 705: {
                    Functions.Log("The all seeing donut knows your current region is " + user_region + "                             ");
                    break;
                }

                case 800: {
                    r = 0;
                    break;
                }

                default: {
                    break;
                }
            }

            //-- wait microseconds
            Functions._usleep(10000);
        }

        //-- kill roblox once time has been met
        Roblox.Kill();

        //-- push the log and then jump to beginning of stack
        Functions.Log("Restarting");
    }

    //-- never gets hit
    system("pause");
}
