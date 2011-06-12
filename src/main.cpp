#include "core/myincapplication.h"
//
int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(resource);
    MyIncApplication app(argc, argv);

    return app.application()->exec();
}
