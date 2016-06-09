#include <iostream>
#include "IDA_Star.h"
#include "SliderGUI.h"

using namespace std;

int main() {
    unsigned int sz = 3;
    do {
        try {
            SliderGUI gui(sz,sz, false);
        } catch (std::exception) { }
        IDA_Star::STOP = true;
        sz = SliderGUI::new_size;
    } while (SliderGUI::new_size > 0);
    return 0;
}