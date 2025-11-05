#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDate>

class Transaction {
public:
    enum Type { Expense = 0, Income = 1};

    QString id;
    QDate date;
    QString category;
    double amount;
    QString description;
    Type type;

    Transaction(QString id = "", QDate date = QDate::currentDate(),
               QString category = "", double amount = 0.0,
               QString description = "", Type type = Expense)
        : id(std::move(id)), date(date), category(std::move(category)),
          amount(amount), description(std::move(description)), type(type) {}

    QString typeString() const {
        return type == Income ? "Income" : "Expense";
    }
};

#endif // TRANSACTION_H