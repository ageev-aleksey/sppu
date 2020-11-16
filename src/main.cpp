#include <QApplication>
#include "gui/QAppWindow.h"
#include "format/FormatsContainer.h"
#include "format/Format.h"

#include <vector>

int main(int argc, char *argv[]) {
    QApplication core(argc, argv);
    QAppWindow window;
    window.show();
    int status = core.exec();
    return status;
}
