#include <iostream>
#include <fstream>
#include <list>
#include <ctime>
#include <chrono>
#include <thread>
#include <time.h>
#include <windows.h>
#include <tchar.h>
#include <psapi.h>

#define FILENAME "blacklist.txt"

using namespace std;

void go_background();
void shows_up();

void get_current_hours(int *hours, int *minutes);
int check_nazi_hour(int hours, int minutes);
bool is_running(string pName);

int main()
{
	ifstream infile;
    string prog;
    list<string> blacklist;
    list<string>::iterator it;
    int hours, minutes, tts;
    bool flag=false;

    //go_background();

	infile.open(FILENAME);
	if(infile.is_open()){
        while(!infile.eof()){
            getline(infile, prog);
            blacklist.push_back(prog);
        }
        infile.close();
	}
	else{
        cout << "Where the fuck is the file " << FILENAME << " ?" << endl;
        return -1;
	}

   while(1){
        get_current_hours(&hours, &minutes);
        tts = check_nazi_hour(hours, minutes);
        if(tts!=1){
            cout << "It's not yet the nazi-hour. Good study. See you later" << endl;
            Sleep(tts);
        }
        else{
            flag = false;
            for(it=blacklist.begin(); it!=blacklist.end() && !flag; it++){
                prog = (*it);
                prog += ".exe";
                if(is_running(prog)){
                    flag = true;
                    cout << "We're in nazi-hour and this program let me think bad (af): " << prog << endl;
                }
            }
            if(flag){
                //shows_up();
                //system("");
            }
        }
   }

    return 0;
}

void go_background()
{
    HWND console = FindWindow("ConsoleWindowClass", NULL);
    if(IsWindow(console))
        ShowWindow(console,SW_HIDE);
}

void shows_up()
{
    HWND console = FindWindow("ConsoleWindowClass", NULL);
    if(IsWindow(console))
        ShowWindow(console, SW_SHOW);

}

void get_current_hours(int *hours, int *minutes)
{
    time_t now;
    struct tm *now_tm;

    now = time(NULL);
    now_tm = localtime(&now);

    (*hours) = now_tm->tm_hour;
    (*minutes) = now_tm->tm_min;
}

int check_nazi_hour(int hours, int minutes)
{
    int tmp, tts=0, h, m;

    tmp = hours*100;
    tmp += minutes;

    if((tmp>1300 && tmp<1500) || (tmp>1900 && tmp<800))
        return 1;

    if(tmp<1900 && tmp>1300){
        h = 19-hours;
        if(minutes!=0)
            h = h-1;
        m = 60-minutes;
    }
    else if(tmp<1300){
        h = 13-hours;
        if(minutes!=0)
            h = h-1;
        m = 60-minutes;
    }

    tts = h*60; //from hours to minutes
    tts += m; //adding minutes

    cout << "I have to sleep " << tts << " minutes" << endl;
    tts *= 1000; //conversion to milliseconds

    return tts;
}

bool is_running(string pName)
{
    unsigned long aProcesses[1024], cbNeeded, cProcesses;
    char buffer[50];

    if(!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return false;

    cProcesses = cbNeeded / sizeof(unsigned long);
    for(unsigned int i = 0; i < cProcesses; i++){
        if(aProcesses[i] == 0)
            continue;

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, aProcesses[i]);
        GetModuleBaseName(hProcess, 0, buffer, 50);
        CloseHandle(hProcess);

        if(pName == string(buffer))
            return true;
    }

    return false;
}
