//
// Created by hefi on 6/4/2025.
//

#include "test.h"
#include <iostream>
#include <cassert>
#include "transactioncontroller.h"
#include "csvtransactionrepository.h"
#include "jsontransactionrepository.h"

void test_add_remove_update() {
    auto repo = std::make_shared<JsonTransactionRepository>("test_transactions.json");
    repo->add(Transaction("t1", QDate::currentDate(), "Food", 20.5, "Lunch", Transaction::Expense));
    assert(repo->exists("t1"));
    TransactionController ctrl(repo);

    ctrl.removeTransaction("t1");
    assert(!repo->exists("t1"));

    Transaction t("t2", QDate::currentDate(), "Salary", 3000, "Monthly pay", Transaction::Income);
    ctrl.addTransaction(t);
    assert(repo->exists("t2"));

    t.amount = 3500;
    ctrl.updateTransaction(t);
    assert(repo->getAll()[0].amount == 3500);

    std::cout << "Add/Remove/Update test passed.\n";
}

void test_undo_redo() {
    auto repo = std::make_shared<JsonTransactionRepository>("test_transactions.json");
    TransactionController ctrl(repo);

    ctrl.addTransaction(Transaction("t3", QDate::currentDate(), "Misc", 100, "Stuff", Transaction::Expense));
    assert(repo->exists("t3"));
    ctrl.undo();
    assert(!repo->exists("t3"));
    ctrl.redo();
    assert(repo->exists("t3"));

    std::cout << "Undo/Redo test passed.\n";
}

void test_filtering() {
    auto repo = std::make_shared<JsonTransactionRepository>("test_transactions.json");
    repo->add(Transaction("f1", QDate::currentDate(), "Groceries", 50, "Supermarket", Transaction::Expense));
    repo->add(Transaction("f2", QDate::currentDate(), "Salary", 2000, "Monthly salary", Transaction::Income));

    TransactionController ctrl(repo);
    auto result = ctrl.filterTransactions([](const Transaction& t) {
        return t.category == "Groceries";
    });

    assert(result.size() == 1);
    assert(result[0].id == "f1");

    std::cout << "Filtering test passed.\n";
}
