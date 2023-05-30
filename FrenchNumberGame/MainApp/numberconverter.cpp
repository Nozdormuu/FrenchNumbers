#include "numberconverter.h"
#include <QFile>
#include <QDebug>

NumberConverter::NumberConverter(QObject *parent)
    : QObject{parent}
{
    LoadFrenchNumbers();
}

void NumberConverter::LoadFrenchNumbers()
{
    QFile file(":/FrenchNumbers.txt");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error when opening file!";
        return;
    }
    while(!file.atEnd())
    {
        QString Input = file.readLine();
        Input = Input.trimmed();
        FrenchNumbersInput.append(Input);
    }
}

QString NumberConverter::NumberToEnglish(uint number)
{
    QVector<QString> TeensEn{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "ninteen"};
    QVector<QString> TensEn{"twenty", "thirty", "fourty", "fifty", "sixty", "seventy", "eighty", "ninety"};
    QString EnglishNumber = "";
    if(number >= 1000000000)
    {
        int billions = number / 1000000000;
        QString s = (number - 1000000000 * billions > 0) ? "and " : "";
        EnglishNumber += NumberToEnglish(billions) + " billion " + s + NumberToEnglish(number - 1000000000 * billions);
    }
    else if(number >= 1000000)
    {
        int millions = number / 1000000;
        QString s = (number - 1000000 * millions > 0) ? "and " : "";
        EnglishNumber += NumberToEnglish(millions) + " million " + s + NumberToEnglish(number - 1000000 * millions);
    }
    else if(number >= 1000)
    {
        int thousands = number / 1000;
        QString s = (number - 1000 * thousands > 0) ? "and " : "";
        EnglishNumber += NumberToEnglish(thousands) + " thousand " + s + NumberToEnglish(number - 1000 * thousands);
    }
    else if(number >= 100)
    {
        int hundreds = number / 100;
        QString s = (number - 100 * hundreds > 0) ? "and " : "";
        EnglishNumber += NumberToEnglish(hundreds) + " hundred " + s + NumberToEnglish(number - 100 * hundreds);
    }
    else if(number >= 20)
    {
        int tens = number / 10;
        EnglishNumber += TensEn[tens - 2] + " " + NumberToEnglish(number - 10 * tens);
    }
    else if(number > 0)
    {
        EnglishNumber += TeensEn[number - 1];
        return EnglishNumber;
    }
    return EnglishNumber.trimmed();
}

QString NumberConverter::NumberToFrench(uint number)
{
    QString FrenchNumber = "";
    if(number >= 1000000000)
    {
        long billions = number / 1000000000;
        QString plural = (billions == 1) ? " " : "s ";
        FrenchNumber += NumberToFrench(billions) + " milliard" + plural + NumberToFrench(number - 1000000000 * billions);
    }
    else if(number >= 1000000)
    {
        int millions = number / 1000000;
        QString plural = (millions == 1) ? " " : "s ";
        FrenchNumber +=  NumberToFrench(millions) + " million"  + plural + NumberToFrench(number - 1000000 * millions);
    }
    else if(number >= 1000)
    {
        int thousands = number / 1000;
        QString prefix = (thousands == 1) ? "" : NumberToFrench(thousands);
        FrenchNumber +=  prefix + " mille " + NumberToFrench(number - 1000 * thousands);
    }
    else if(number >= 100)
    {
        int hundreds = number / 100;
        QString prefix = (hundreds == 1) ? "" : FrenchNumbersInput[hundreds] + " ";
        FrenchNumber +=  prefix + "cent " + NumberToFrench(number - 100 * hundreds);
    }
    else if(number > 0)
    {
        FrenchNumber += FrenchNumbersInput[number];
    }
    return FrenchNumber.trimmed();
}

NumberConverter::~NumberConverter() {};
