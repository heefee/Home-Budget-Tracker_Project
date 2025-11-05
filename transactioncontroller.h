#ifndef TRANSACTIONCONTROLLER_H
#define TRANSACTIONCONTROLLER_H

#include "transaction.h"
#include "transactionrepository.h"
#include "command.h"
#include <QObject>
#include <memory>
#include <functional>

class TransactionController : public QObject {
    Q_OBJECT

    std::shared_ptr<TransactionRepository> repository;
    CommandManager commandManager;

public:
    explicit TransactionController(std::shared_ptr<TransactionRepository> repo,
                                 QObject* parent = nullptr);

    QVector<Transaction> getAllTransactions() const;
    QVector<Transaction> filterTransactions(std::function<bool(const Transaction&)> predicate) const;

    void addTransaction(const Transaction& t);
    void removeTransaction(const QString& id);
    void updateTransaction(const Transaction& t);

    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;

signals:
    void dataChanged();
};

#endif // TRANSACTIONCONTROLLER_H