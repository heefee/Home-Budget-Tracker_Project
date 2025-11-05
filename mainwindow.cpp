#include "mainwindow.h"
#include "csvtransactionrepository.h"
#include "jsontransactionrepository.h"
#include "filterdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent) {
    repository = std::make_shared<JsonTransactionRepository>("transactions.json");
    controller = new TransactionController(repository, this);
    
    setupUI();
    setupMenu();
    setupConnections();
    refreshTable();
}

MainWindow::~MainWindow() {
    delete controller;
}

void MainWindow::setupUI() {
    setWindowTitle("Home Budget Tracker");
    resize(800, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Form layout
    auto* formLayout = new QFormLayout();
    idEdit = new QLineEdit();
    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    categoryEdit = new QLineEdit();
    amountEdit = new QLineEdit();
    descriptionEdit = new QLineEdit();
    incomeCheck = new QCheckBox("Income");

    formLayout->addRow("ID:", idEdit);
    formLayout->addRow("Date:", dateEdit);
    formLayout->addRow("Category:", categoryEdit);
    formLayout->addRow("Amount:", amountEdit);
    formLayout->addRow("Description:", descriptionEdit);
    formLayout->addRow("Type:", incomeCheck);

    auto* buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("Add");
    deleteButton = new QPushButton("Delete");
    updateButton = new QPushButton("Update");
    filterButton = new QPushButton("Filter");
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(filterButton);

    auto* formatLayout = new QHBoxLayout();
    formatCombo = new QComboBox();
    formatCombo->addItems({"JSON", "CSV"});
    formatLayout->addWidget(new QLabel("Format:"));
    formatLayout->addWidget(formatCombo);
    formatLayout->addStretch();

    table = new QTableWidget(0, 6);
    table->setHorizontalHeaderLabels({"ID", "Date", "Category", "Amount", "Description", "Type"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    auto* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(formatLayout);
    mainLayout->addWidget(table);

    QPushButton* resetFilterButton = new QPushButton("Reset Filter");
    buttonLayout->addWidget(resetFilterButton);
    connect(resetFilterButton, &QPushButton::clicked, this, &MainWindow::refreshTable);
}

void MainWindow::setupConnections() {
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateClicked);
    connect(filterButton, &QPushButton::clicked, this, &MainWindow::onFilter);
    connect(formatCombo, &QComboBox::currentTextChanged, this, &MainWindow::onFormatChanged);
    connect(controller, &TransactionController::dataChanged, this, &MainWindow::refreshTable);
}

void MainWindow::setupMenu() {
    auto* editMenu = menuBar()->addMenu("Edit");
    undoAction = editMenu->addAction("Undo");
    redoAction = editMenu->addAction("Redo");

    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onRedo);
}


void MainWindow::onAddClicked() {
    Transaction t = createTransactionFromForm();
    if (!t.id.isEmpty() && !t.category.isEmpty() && t.amount > 0) {
        controller->addTransaction(t);
        clearForm();
    } else {
        QMessageBox::warning(this, "Invalid Data", "Please fill all required fields with valid data");
    }
}

void MainWindow::onDeleteClicked() {
    int row = table->currentRow();
    if (row >= 0) {
        QString id = table->item(row, 0)->text();
        controller->removeTransaction(id);
    }
}

void MainWindow::onUpdateClicked() {
    int row = table->currentRow();
    if (row >= 0) {
        QString id = table->item(row, 0)->text();
        Transaction t = createTransactionFromForm();
        t.id = id; // Keep the same ID
        controller->updateTransaction(t);
        clearForm();
    }
}

void MainWindow::onUndo() {
    controller->undo();
}

void MainWindow::onRedo() {
    controller->redo();
}


void MainWindow::onFormatChanged() {
    QString currentFile = formatCombo->currentText() == "JSON" ? "transactions.json" : "transactions.csv";
    if (formatCombo->currentText() == "JSON") {
        repository = std::make_shared<JsonTransactionRepository>(currentFile);
    } else {
        repository = std::make_shared<CsvTransactionRepository>(currentFile);
    }
    controller = new TransactionController(repository, this);
    connect(controller, &TransactionController::dataChanged, this, &MainWindow::refreshTable);
    refreshTable();
}

void MainWindow::refreshTable() {
    displayTransactions(controller->getAllTransactions());
}


Transaction MainWindow::createTransactionFromForm() const {
    return Transaction(
        idEdit->text(),
        dateEdit->date(),
        categoryEdit->text(),
        amountEdit->text().toDouble(),
        descriptionEdit->text(),
        incomeCheck->isChecked() ? Transaction::Income : Transaction::Expense
    );
}

void MainWindow::clearForm() {
    idEdit->clear();
    dateEdit->setDate(QDate::currentDate());
    categoryEdit->clear();
    amountEdit->clear();
    descriptionEdit->clear();
    incomeCheck->setChecked(false);
}

void MainWindow::onFilter() {
    FilterDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        auto criteria = dialog.getFilterCriteria();
        auto filteredTransactions = controller->filterTransactions(criteria);
        displayTransactions(filteredTransactions);
    }
}

void MainWindow::displayTransactions(const QVector<Transaction>& transactions) {
    table->setRowCount(0);
    for (int i = 0; i < transactions.size(); ++i) {
        const auto& t = transactions[i];
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(t.id));
        table->setItem(i, 1, new QTableWidgetItem(t.date.toString("yyyy-MM-dd")));
        table->setItem(i, 2, new QTableWidgetItem(t.category));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(t.amount)));
        table->setItem(i, 4, new QTableWidgetItem(t.description));
        table->setItem(i, 5, new QTableWidgetItem(t.typeString()));
    }
}

