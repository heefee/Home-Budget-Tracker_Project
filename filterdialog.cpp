#include "filterdialog.h"

#include <QDate>
#include <QFormLayout>
#include <QMessageBox>
#include <QToolBar>
#include <QFileDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
FilterDialog::FilterDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Filter Transactions");
    
    QDate today = QDate::currentDate();
    fromDateEdit = new QDateEdit(today.addMonths(-1));
    toDateEdit = new QDateEdit(today);
    categoryEdit = new QLineEdit();
    minAmountEdit = new QLineEdit();
    maxAmountEdit = new QLineEdit();
    typeCombo = new QComboBox();
    typeCombo->addItems({"All", "Income", "Expense"});
    
    dateCheck = new QCheckBox("Use date range");
    categoryCheck = new QCheckBox("Filter by category");
    amountCheck = new QCheckBox("Filter by amount");
    typeCheck = new QCheckBox("Filter by type");
    
    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    
    auto* layout = new QFormLayout(this);
    layout->addRow(dateCheck, fromDateEdit);
    layout->addRow("To:", toDateEdit);
    layout->addRow(categoryCheck, categoryEdit);
    layout->addRow(amountCheck, minAmountEdit);
    layout->addRow("To:", maxAmountEdit);
    layout->addRow(typeCheck, typeCombo);
    layout->addWidget(buttonBox);
}

FilterDialog::FilterCriteria FilterDialog::getFilterCriteria() const {
    FilterCriteria criteria;
    
    criteria.useDateRange = dateCheck->isChecked();
    if (criteria.useDateRange) {
        criteria.fromDate = fromDateEdit->date();
        criteria.toDate = toDateEdit->date();
    }
    
    criteria.useCategory = categoryCheck->isChecked();
    if (criteria.useCategory) {
        criteria.category = categoryEdit->text();
    }
    
    criteria.useAmountRange = amountCheck->isChecked();
    if (criteria.useAmountRange) {
        criteria.minAmount = minAmountEdit->text().toDouble();
        criteria.maxAmount = maxAmountEdit->text().toDouble();
    }
    
    criteria.useType = typeCheck->isChecked();
    if (criteria.useType) {
        criteria.type = static_cast<Transaction::Type>(typeCombo->currentIndex());
    }
    
    return criteria;
}