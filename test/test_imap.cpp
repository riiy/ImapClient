#include <QTest>
#include "../src/imapclient.h"
class TestImap: public QObject
{
    Q_OBJECT

private:
    bool myCondition()
    {
        return true;
    }

    QString username = "bo.zhou@bua.edc.cn";
    QString password = "";
    ImapClient imap;
private slots:
    void initTestCase()
    {
        QFile file("/Users/lld/ImapClient/mail_auth.conf");
            bool isOk = file.open(QIODevice::ReadOnly);
            if(isOk==true){
                //QDataStream stream(&file);//读取二进制文件
                QTextStream stream(&file);
                stream >> password;
              }else{
                        qDebug()<<"打开失败";
                    }
        qDebug("Called before everything else.");
    }

    void myFirstTest()
    {
        ImapClient imap("imap.exmail.qq.com", 993, ImapClient::START_TLS);
        imap.connect_host();
        imap.authenticate(username, password);
        QVERIFY(true); // check that a condition is satisfied
        QCOMPARE(1, 1); // compare two values
    }

    void mySecondTest()
    {
        QVERIFY(myCondition());
        QVERIFY(1 != 2);
    }

    void cleanupTestCase()
    {
        qDebug("Called after myFirstTest and mySecondTest.");
    }
};
QTEST_MAIN(TestImap)
#include "test_imap.moc"
