#ifndef JSONTRANSACTIONREPOSITORY_H
#define JSONTRANSACTIONREPOSITORY_H

#include "transactionrepository.h"
#include <QString>

class JsonTransactionRepository : public TransactionRepository {
    QString filePath;
    QVector<Transaction> transactions;

public:
    explicit JsonTransactionRepository(const QString& path);
    QVector<Transaction> getAll() const override;
    void add(const Transaction& t) override;
    void remove(const QString& id) override;
    void update(const Transaction& t) override;
    bool exists(const QString& id) const override;
    void save() override;
    void load() override;
};

#endif // JSONTRANSACTIONREPOSITORY_H