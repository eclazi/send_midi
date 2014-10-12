//
//  main.cpp
//  send_midi
//
//  Created by Joss Gray on 12/10/2014.
//  Copyright (c) 2014 Joss Gray. All rights reserved.
//

#include "rtmidi.h"

#include <iostream>
#include <memory>

using namespace std;

// Decode ASCII hex to uchar. E.g '0A' becomes 0x0A
unsigned char hexDecode(const std::string& s)
{
    unsigned char res = 0;
    for (size_t i = 0; i < s.size(); i++)
    {
        char c = s.c_str()[i];
        if (c >= '0' && c <= '9')
        {
            res |= (c - '0') << (8 * (1 - i));
        }
        else if (c >= 'A' && c <= 'F')
        {
            res |= (c - 'A' + 10) << (8 * (1 - i));
        }
    }
    return res;
}

int main(int argc, const char * argv[])
{
    shared_ptr<RtMidiOut> midiOut;
    
    try
    {
        midiOut = make_shared<RtMidiOut>();
    } catch (RtMidiError& error)
    {
        error.printMessage();
        return -1;
    }
    
    if (argc == 1)
    {
        for (unsigned int i = 0; i < midiOut->getPortCount(); i++)
        {
            try
            {
                cout << i << " : " << midiOut->getPortName(i) << "\n";
            } catch (RtMidiError& error)
            {
                error.printMessage();
                return -1;
            }
        }
        return 0;
    }
    
    if (argc == 3)
    {
        int port = atoi(argv[1]);
        string bytes_str = argv[2];
        vector<unsigned char> bytes;
        
        for (size_t i = 0; i < bytes_str.size(); i+=2)
        {
            unsigned char b = hexDecode(bytes_str.substr(i, 2));
            bytes.push_back(b);
        }
        
        try
        {
            midiOut->openPort(port);
        } catch (RtMidiError& error)
        {
            error.printMessage();
            return -1;
        }

        midiOut->sendMessage(&bytes);
    }
    return 0;
}
