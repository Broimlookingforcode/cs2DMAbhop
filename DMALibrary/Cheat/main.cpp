#include "pch.h" // Include the precompiled header

#include <iostream>
#include "Memory/Memory.h""
#include "kmbox_interface.hpp"
#include <windows.h> 
 

inline void km_space() {

    std::string command = "km.space(" + std::to_string(1) + ")\r\n"; // space bar down
    send_command(hSerial, command.c_str());

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::string command1 = "km.space(" + std::to_string(0) + ")\r\n"; // space bar up 
    send_command(hSerial, command1.c_str());
}
    
void bunny_hop(uintptr_t client)
{
    uintptr_t local_pawn = mem.Read<uintptr_t>(client + 0x1823A08);

    if (!local_pawn)
        return;

    // TO-CHECK @ maybe need read from local local_controller, not local_pawn, not tested
    int32_t flags = mem.Read<int32_t>(local_pawn + 0x3CC);  

     if (mem.GetKeyboard()->IsKeyDown(0x20) && (flags & (1 << 0))) 
         km_space(); 
}

int main() {
    // Find and open the kmbox port
    std::string port = find_port("USB-SERIAL CH340"); // name of the kmbox port without ( COM )
    if (port.empty())  
    {
        MessageBoxA(0, "No KMBOX Connection, Auto - Close", 0, 0);
        exit(0);
    }

    if (!open_port(hSerial, port.c_str(), CBR_115200)) 
    {
        MessageBoxA(0, "No KMBOX Connection, Auto - Close", 0, 0);
        exit(0);
    }
     
    std::cout << "\n\t[+] Connected to the kmbox with " + port << std::endl;
     
    if (!mem.Init("cs2.exe", true, false))
    {
        MessageBoxA(0, "No DMA Connection, Auto - Close", 0, 0);
        exit(0);
    }
    else
    {
        std::cout << "DMA initialized" << std::endl;
    }
     
    if (!mem.GetKeyboard()->InitKeyboard())  
    {
        MessageBoxA(0, "No Hotkeys, Auto - Close", 0, 0);
        exit(0);
    }

    uintptr_t base = mem.GetBaseDaddy("client.dll");
    
    if (!base)
    {
        MessageBoxA(0, "No Client Base Adress, Auto - Close", 0, 0);
        exit(0);
    }

    while (true)
    { 
        bunny_hop(base); 
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

