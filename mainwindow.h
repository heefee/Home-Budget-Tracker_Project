#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "transactioncontroller.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QDateEdit;
class QCheckBox;
class QTableWidget;
class QPushButton;
class QComboBox;
class QAction;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddClicked();
    void onDeleteClicked();
    void onUpdateClicked();
    void onUndo();
    void onRedo();
    void onFilter();
    void onFormatChanged();
    void refreshTable();

private:
    void setupUI();
    void setupMenu();
    void setupConnections();
    Transaction createTransactionFromForm() const;
    void clearForm();
    void displayTransactions(const QVector<Transaction>& transactions);


    QLineEdit* idEdit;
    QDateEdit* dateEdit;
    QLineEdit* categoryEdit;
    QLineEdit* amountEdit;
    QLineEdit* descriptionEdit;
    QCheckBox* incomeCheck;
    QTableWidget* table;
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* updateButton;
    QPushButton* filterButton;
    QComboBox* formatCombo;
    QAction* undoAction;
    QAction* redoAction;

    std::shared_ptr<TransactionRepository> repository;
    TransactionController* controller;
};

#endif // MAINWINDOW_H