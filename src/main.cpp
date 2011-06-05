#include "core/myincapplication.h"
//
int main(int argc, char *argv[])
{
    MyIncApplication app(argc, argv);

    return app.application()->exec();
}
