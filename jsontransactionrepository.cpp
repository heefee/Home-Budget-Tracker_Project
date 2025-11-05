#include "jsontransactionrepository.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

JsonTransactionRepository::JsonTransactionRepository(const QString& path) 
    : filePath(path) {
    load();
}

QVector<Transaction> JsonTransactionRepository::getAll() const {
    return transactions;
}

void JsonTransactionRepository::add(const Transaction& t) {
    if (!exists(t.id)) {
        transactions.append(t);
        save();
    }
}

void JsonTransactionRepository::remove(const QString& id) {
    transactions.erase(std::remove_if(transactions.begin(), transactions.end(),
        [&id](const Transaction& t) { return t.id == id; }), transactions.end());
    save();
}

void JsonTransactionRepository::update(const Transaction& t) {
    auto it = std::find_if(transactions.begin(), transactions.end(),
        [&t](const Transaction& tr) { return tr.id == t.id; });
    if (it != transactions.end()) {
        *it = t;
        save();
    }
}

bool JsonTransactionRepository::exists(const QString& id) const {
    return std::any_of(transactions.begin(), transactions.end(),
        [&id](const Transaction& t) { return t.id == id; });
}

void JsonTransactionRepository::save() {
    QJsonArray array;
    for (const auto& t : transactions) {
        QJsonObject obj;
        obj["id"] = t.id;
        obj["date"] = t.date.toString("yyyy-MM-dd");
        obj["category"] = t.category;
        obj["amount"] = t.amount;
        obj["description"] = t.description;
        obj["type"] = static_cast<int>(t.type);
        array.append(obj);
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonDocument doc(array);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

void JsonTransactionRepository::load() {
    transactions.clear();
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonArray array = doc.array();

        for (const auto& val : array) {
            QJsonObject obj = val.toObject();
            Transaction t;
            t.id = obj["id"].toString();
            t.date = QDate::fromString(obj["date"].toString(), "yyyy-MM-dd");
            t.category = obj["category"].toString();
            t.amount = obj["amount"].toDouble();
            t.description = obj["description"].toString();
            t.type = static_cast<Transaction::Type>(obj["type"].toInt());

            transactions.append(t);
        }
        file.close();
    }
}