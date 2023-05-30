#include <QtTest>
#include <QCoreApplication>
#include "../MainApp/numberconverter.h"

class TestNumberConversion :  public QObject
{
    Q_OBJECT

public:
    NumberConverter numberConverter;

private slots:
    void TestNumberToFrenchWord();
    void TestNumberToEnglishWord();
};


void TestNumberConversion::TestNumberToFrenchWord()
{
    QCOMPARE(numberConverter.NumberToFrench(5), QString("cinq"));
    QCOMPARE(numberConverter.NumberToFrench(197), QString("cent quatre vingt dix sept"));
    QCOMPARE(numberConverter.NumberToFrench(97000), QString("quatre vingt dix sept mille"));
    QCOMPARE(numberConverter.NumberToFrench(197000), QString("cent quatre vingt dix sept mille"));
    QCOMPARE(numberConverter.NumberToFrench(900000000), QString("neuf cent millions"));
    QCOMPARE(numberConverter.NumberToFrench(2000000824), QString("deux milliards huit cent vingt quatre"));
}

void TestNumberConversion::TestNumberToEnglishWord()
{
    QCOMPARE(numberConverter.NumberToEnglish(1), QString("one"));
    QCOMPARE(numberConverter.NumberToEnglish(197), QString("one hundred and ninety seven"));
    QCOMPARE(numberConverter.NumberToEnglish(97000), QString("ninety seven thousand"));
    QCOMPARE(numberConverter.NumberToEnglish(197000), QString("one hundred and ninety seven thousand"));
    QCOMPARE(numberConverter.NumberToEnglish(900000000), QString("nine hundred million"));
    QCOMPARE(numberConverter.NumberToEnglish(2000000824), QString("two billion and eight hundred and twenty four"));
}

QTEST_MAIN(TestNumberConversion)
#include "tst_testnumberconversion.moc"
