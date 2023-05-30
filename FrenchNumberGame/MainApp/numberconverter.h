#ifndef NUMBERCONVERTER_H
#define NUMBERCONVERTER_H

#include <QObject>

    /*
 * @brief Класс перевода чисел
 */
    class NumberConverter : public QObject
{
    Q_OBJECT
public:

    /*
    * @brief конструктор
    */
    explicit NumberConverter(QObject* parent = nullptr);

    /*
   * @brief функция перевод на английский
   */
    QString NumberToEnglish(uint number);

    /*
   * @brief функция перевода на французский
   */
    QString NumberToFrench(uint number);

    /*
    * @brief деструктор
    */
    ~NumberConverter();

private:
    QVector<QString> FrenchNumbersInput;

    /*
    * @brief функция загрузки
    */
    void LoadFrenchNumbers();
};

#endif // NUMBERCONVERTER_H
