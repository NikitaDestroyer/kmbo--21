﻿#include <iostream>
#include "electricity.h"

using namespace std;


int main()
{
    Switch sw, sw2;
    sw.connect("A2", sw2, "A1");
    cout << "is " << (sw.isConnectedTo(sw2) ? " " : "not ") << "connected" << endl;
    sw.disconnect("A1");
    cout << "is " << (sw.isConnectedTo(sw2) ? " " : "not ") << "connected" << endl;
    
    cout << "Other::::::\n";
    Switch sw1;
    Light lamp;
    Generator generator;
    //Генератор [фаза] - выключатель - лампа - [нейтраль] Генератор
    generator.connect("A1", sw1, "A2"); 
    sw1.connect("A1", lamp, "A2"); 
    lamp.connect("A2", generator, "A2");
    cout << "is " << (sw1.isConnectedTo(lamp) ? " " : "not ") << "connected" << endl;
    generator.disconnect("A1");
    cout << "is " << (generator.isConnectedTo(sw1) ? " " : "not ") << "connected" << endl;

    return 0;
}
