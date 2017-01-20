#include <cstdlib>

#include "jumpy.hpp"

int main(int argc, char *argv[])
{

    Jumpy *jumpy;
    jumpy = Jumpy::getInstance();

    jumpy->start();

    return 0;
}
