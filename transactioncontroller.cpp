#include "transactioncontroller.h"
#include "transactioncommands.h"

TransactionController::TransactionController(std::shared_ptr<TransactionRepository> repo, QObject* parent)
    : QObject(parent), repository(std::move(repo)) {}

QVector<Transaction> TransactionController::getAllTransactions() const {
    return repository->getAll();
}

QVector<Transaction> TransactionController::filterTransactions(std::function<bool(const Transaction&)> predicate) const {
    QVector<Transaction> result;
    for (const auto& t : repository->getAll()) {
        if (predicate(t)) {
            result.append(t);
        }
    }
    return result;
}

void TransactionController::addTransaction(const Transaction& t) {
    commandManager.execute(std::make_unique<AddTransactionCommand>(repository, t));
    emit dataChanged();
}

void TransactionController::removeTransaction(const QString& id) {
    auto transactions = repository->getAll();
    auto it = std::find_if(transactions.begin(), transactions.end(),
        [&id](const Transaction& t) { return t.id == id; });
    if (it != transactions.end()) {
        commandManager.execute(std::make_unique<RemoveTransactionCommand>(repository, *it));
        emit dataChanged();
    }
}

void TransactionController::updateTransaction(const Transaction& t) {
    if (repository->exists(t.id)) {
        auto oldTransaction = *std::find_if(repository->getAll().begin(), repository->getAll().end(),
            [&t](const Transaction& tr) { return tr.id == t.id; });
        commandManager.execute(std::make_unique<UpdateTransactionCommand>(repository, oldTransaction, t));
        emit dataChanged();
    }
}

void TransactionController::undo() {
    commandManager.undo();
    emit dataChanged();
}

void TransactionController::redo() {
    commandManager.redo();
    emit dataChanged();
}

bool TransactionController::canUndo() const {
    return commandManager.canUndo();
}

bool TransactionController::canRedo() const {
    return commandManager.canRedo();
}