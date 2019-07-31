#include "TemplateHandler.h"

#include "qt_utils.h"

#include <QDebug>
#include <QListView>
#include <QModelIndex>
#include <QObject>
#include <QStringList>
#include <QStringListModel>
#include <QTextCursor>
#include <QTextEdit>

#include <algorithm>
#include <cassert>
#include <memory>

struct TemplateHandler::impl
{
    impl(QStringListModel* model, QListView* view, QTextEdit* textEdit)
        :
          Model_(model),
          View_(view),
          TextEdit_(textEdit)
    {}

    QStringListModel* Model_;
    QListView* View_;
    QTextEdit* TextEdit_;
};

namespace
{
void fill_model(QStringListModel& model)
{
    QStringList templates({"head", "html", "body", "p"});
    std::sort(std::begin(templates), std::end(templates));
    model.setStringList(templates);
}
}

TemplateHandler::TemplateHandler(QListView* view, QTextEdit* textEdit, QObject* parent)
    :
      base(parent),
      Impl_(std::make_unique<impl>(new QStringListModel(this), view, textEdit))
{
    assert(nullptr != this->Impl_->Model_);
    assert(nullptr != this->Impl_->View_);
    assert(nullptr != this->Impl_->TextEdit_);

    fill_model(*this->Impl_->Model_);
    this->Impl_->View_->setModel(this->Impl_->Model_);

    SAFE_CONNECT(this->Impl_->View_, SIGNAL(clicked(const QModelIndex&)), this, SLOT(insertText(const QModelIndex&)));
}

TemplateHandler::TemplateHandler(QListView* view, QTextEdit* textEdit)
    : TemplateHandler(view, textEdit, nullptr)
{ }

TemplateHandler::~TemplateHandler() = default;

void TemplateHandler::insertText(const QModelIndex& index)
{
    auto text = this->Impl_->Model_->data(index, Qt::DisplayRole).toString();
    auto cursor = this->Impl_->TextEdit_->textCursor();

    auto beginTag = "<" + text +">";
    auto endTag = "</" + text +">";
    cursor.insertText(beginTag + endTag);

    auto position = cursor.position() - endTag.length();
    assert(0 <= position);
    cursor.setPosition(position);

    this->Impl_->TextEdit_->setTextCursor(cursor);
}
