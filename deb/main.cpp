#include <iostream>
#include <windows.h>

using namespace std;

int main()
{

    //Запуск приложения
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    wchar_t cmdLine[] = L"D:/GitExam/201_331_Egel/build-201_331_Egel-Desktop_Qt_6_4_2_MSVC2019_64bit-Debug/debug/201_331_Egel.exe";


    if(CreateProcess(cmdLine, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) {
        std::cout << "Process id created !" << std::endl;
        std::cout << "Pid = " << std::dec << pi.dwProcessId << std::endl;
    }
    else {
        std::cout << "Process is not created!"<< std::endl; //Процесс не запустился
    }

    //Подключение к приложению как отладчик

    bool isAttached = DebugActiveProcess(pi.dwProcessId);
    if(!isAttached) {
        DWORD lastError = GetLastError();
        std::cout << std::hex << "DebugActiveProcess is FAILED, LastError is " << lastError;
    }
    else {
        std::cout << "DebugActiveProcess is success" << std::endl;
    }

    // Пропускать поступающие сигналы отладки

    DEBUG_EVENT debugEvent;
    while(true) {
        DWORD result = WaitForSingleObject(pi.hProcess, 1); //Статус процесса

        // Проверяем результат
        if (result == WAIT_OBJECT_0) {
            // Процесс завершился, выходим из цикла
            break;
        }
        else {

            bool result1 = WaitForDebugEvent(&debugEvent, INFINITE);
            bool result2 = ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, DBG_CONTINUE);
        }
    }
    std::cout << "Process is stoped!"<< std::endl;

    return 0;

}
