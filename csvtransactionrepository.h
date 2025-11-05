#ifndef CSVTRANSACTIONREPOSITORY_H
#define CSVTRANSACTIONREPOSITORY_H

#include "transactionrepository.h"
#include <QString>

class CsvTransactionRepository : public TransactionRepository {
    QString filePath;
    QVector<Transaction> transactions;

public:
    explicit CsvTransactionRepository(const QString& path);
    QVector<Transaction> getAll() const override;
    void add(const Transaction& t) override;
    void remove(const QString& id) override;
    void update(const Transaction& t) override;
    bool exists(const QString& id) const override;
    void save() override;
    void load() override;
};

#endif // CSVTRANSACTIONREPOSITORY_H