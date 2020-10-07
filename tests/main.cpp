#include "tst_sqldatabase.h"
#include "tst_asyncsqldatabase.h"

#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    // Start QCoreApplication to use signals & slots
    // https://forum.qt.io/topic/84229/is-there-a-canonical-way-to-set-up-qapplication-and-google-test-together/3
    QCoreApplication app{argc, argv};

    QTimer::singleShot(0, [&]()
    {
        ::testing::InitGoogleTest(&argc, argv);
        auto testResult = RUN_ALL_TESTS();
        app.exit(testResult);
    });

    return app.exec();
}
