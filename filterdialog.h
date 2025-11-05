#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include "transaction.h"

QT_BEGIN_NAMESPACE
class QDateEdit;
class QLineEdit;
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

class FilterDialog : public QDialog {
    Q_OBJECT

public:
    explicit FilterDialog(QWidget* parent = nullptr);
    
    struct FilterCriteria {
        bool useDateRange = false;
        QDate fromDate;
        QDate toDate;

        bool useCategory = false;
        QString category;

        bool useAmountRange = false;
        double minAmount = 0.0;
        double maxAmount = 0.0;

        bool useType = false;
        Transaction::Type type;

        bool operator()(const Transaction& t) const {
            if (useDateRange && (t.date < fromDate || t.date > toDate))
                return false;
            if (useCategory && t.category != category)
                return false;
            if (useAmountRange && (t.amount < minAmount || t.amount > maxAmount))
                return false;
            if (useType && t.type != type)
                return false;
            return true;
        }
    };


    FilterCriteria getFilterCriteria() const;

private:
    QDateEdit* fromDateEdit;
    QDateEdit* toDateEdit;
    QLineEdit* categoryEdit;
    QLineEdit* minAmountEdit;
    QLineEdit* maxAmountEdit;
    QComboBox* typeCombo;
    QCheckBox* dateCheck;
    QCheckBox* categoryCheck;
    QCheckBox* amountCheck;
    QCheckBox* typeCheck;
};

#endif // FILTERDIALOG_H