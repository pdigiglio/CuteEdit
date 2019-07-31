#pragma once

class QListView;
class QModelIndex;
class QTextEdit;

#include <QObject>

#include <memory>

class TemplateHandler : public QObject
{
    Q_OBJECT

public:
    explicit TemplateHandler(QListView* view, QTextEdit* textEdit, QObject* parent);
    explicit TemplateHandler(QListView* view, QTextEdit* textEdit);
    ~TemplateHandler() override;

    TemplateHandler(const TemplateHandler&) = delete;
    TemplateHandler(TemplateHandler&&) = delete;

    TemplateHandler& operator=(const TemplateHandler&) = delete;
    TemplateHandler& operator=(TemplateHandler&&) = delete ;

private slots:
    void insertText(const QModelIndex& index);

private:
    using base = QObject;

    struct impl;
    std::unique_ptr<impl> Impl_;
};
