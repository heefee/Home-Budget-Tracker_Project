#ifndef TRANSACTIONCOMMANDS_H
#define TRANSACTIONCOMMANDS_H

#include "transaction.h"
#include "transactionrepository.h"
#include <memory>

class AddTransactionCommand : public Command {
    std::shared_ptr<TransactionRepository> repository;
    Transaction transaction;

public:
    AddTransactionCommand(std::shared_ptr<TransactionRepository> repo, const Transaction& t)
        : repository(std::move(repo)), transaction(t) {}

    void execute() override {
        repository->add(transaction);
    }

    void undo() override {
        repository->remove(transaction.id);
    }
};

class RemoveTransactionCommand : public Command {
    std::shared_ptr<TransactionRepository> repository;
    Transaction transaction;

public:
    RemoveTransactionCommand(std::shared_ptr<TransactionRepository> repo, const Transaction& t)
        : repository(std::move(repo)), transaction(t) {}

    void execute() override {
        repository->remove(transaction.id);
    }

    void undo() override {
        repository->add(transaction);
    }
};

class UpdateTransactionCommand : public Command {
    std::shared_ptr<TransactionRepository> repository;
    Transaction oldTransaction;
    Transaction newTransaction;

public:
    UpdateTransactionCommand(std::shared_ptr<TransactionRepository> repo, 
                           const Transaction& oldT, const Transaction& newT)
        : repository(std::move(repo)), oldTransaction(oldT), newTransaction(newT) {}

    void execute() override {
        repository->update(newTransaction);
    }

    void undo() override {
        repository->update(oldTransaction);
    }
};

#endif // TRANSACTIONCOMMANDS_H