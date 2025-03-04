#include "window.h"


int main() 
{
    Window window;
    if (!window.init(640, 480))
    {
        return 0;
    }
    
    return 0;
}