#include <QtTest>
#include <QCoreApplication>
#include "Test_QSddModelJsonSerializer.h"

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);
    QTest::qExec(new Test_QSddModelJsonSerializer);
    return  0;
}