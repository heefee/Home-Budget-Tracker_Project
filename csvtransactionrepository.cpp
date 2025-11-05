#include "csvtransactionrepository.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

CsvTransactionRepository::CsvTransactionRepository(const QString& path) 
    : filePath(path) {
    load();
}

QVector<Transaction> CsvTransactionRepository::getAll() const {
    return transactions;
}

void CsvTransactionRepository::add(const Transaction& t) {
    if (!exists(t.id)) {
        transactions.append(t);
        save();
    }
}

void CsvTransactionRepository::remove(const QString& id) {
    transactions.erase(std::remove_if(transactions.begin(), transactions.end(),
        [&id](const Transaction& t) { return t.id == id; }), transactions.end());
    save();
}

void CsvTransactionRepository::update(const Transaction& t) {
    auto it = std::find_if(transactions.begin(), transactions.end(),
        [&t](const Transaction& tr) { return tr.id == t.id; });
    if (it != transactions.end()) {
        *it = t;
        save();
    }
}

bool CsvTransactionRepository::exists(const QString& id) const {
    return std::any_of(transactions.begin(), transactions.end(),
        [&id](const Transaction& t) { return t.id == id; });
}

void CsvTransactionRepository::save() {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto& t : transactions) {
            out << t.id << "," << t.date.toString("yyyy-MM-dd") << ","
                << t.category << "," << t.amount << ","
                << t.description << "," << static_cast<int>(t.type) << "\n";
        }
        file.close();
    }
}

void CsvTransactionRepository::load() {
    transactions.clear();
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 6) {
                Transaction t;
                t.id = parts[0];
                t.date = QDate::fromString(parts[1], "yyyy-MM-dd");
                t.category = parts[2];
                t.amount = parts[3].toDouble();
                t.description = parts[4];
                t.type = static_cast<Transaction::Type>(parts[5].toInt());
                transactions.append(t);
            }
        }
        file.close();
    }
}
