#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include "transaction.h"
#include <QVector>
#include <memory>

class TransactionRepository {
public:
    virtual ~TransactionRepository() = default;
    virtual QVector<Transaction> getAll() const = 0;
    virtual void add(const Transaction& t) = 0;
    virtual void remove(const QString& id) = 0;
    virtual void update(const Transaction& t) = 0;
    virtual void save() = 0;
    virtual void load() = 0;
    virtual bool exists(const QString& id) const = 0;
};

#endif // TRANSACTIONREPOSITORY_H