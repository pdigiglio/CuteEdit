#include "MainWindow.h"

#include "qt_utils.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>

#include <cassert>
#include <memory>

struct MainWindow::impl
{
    impl()
        :
          Ui_(),
          CurrentFilePath_(),
          StatusBarMessageTimeoutMs_(3000)
    {}

    Ui::MainWindow Ui_;
    QString CurrentFilePath_;
    int StatusBarMessageTimeoutMs_;
};

MainWindow::MainWindow(QWidget *parent)
    :
      base(parent),
      Impl_(std::make_unique<impl>())
{
    this->Impl_->Ui_.setupUi(this);
    this->setWindowTitle(this->tr("CuteEdit"));

    auto& ui = this->Impl_->Ui_;
    SAFE_CONNECT(ui.actionNew,    SIGNAL(triggered(bool)), this, SLOT(loadNew()));
    SAFE_CONNECT(ui.actionOpen,   SIGNAL(triggered(bool)), this, SLOT(loadFile()));
    SAFE_CONNECT(ui.actionSave,   SIGNAL(triggered(bool)), this, SLOT(saveFile()));
    SAFE_CONNECT(ui.actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(saveFileAs()));

    SAFE_CONNECT(ui.textEdit, SIGNAL(copyAvailable(bool)), ui.actionCopy, SLOT(setEnabled(bool)));
    SAFE_CONNECT(ui.textEdit, SIGNAL(undoAvailable(bool)), ui.actionUndo, SLOT(setEnabled(bool)));
    SAFE_CONNECT(ui.textEdit, SIGNAL(redoAvailable(bool)), ui.actionRedo, SLOT(setEnabled(bool)));

    SAFE_CONNECT(ui.actionCut,   SIGNAL(triggered(bool)), this, SLOT(cut()));
    SAFE_CONNECT(ui.actionPaste, SIGNAL(triggered(bool)), this, SLOT(paste()));
    SAFE_CONNECT(ui.actionCopy,  SIGNAL(triggered(bool)), this, SLOT(copy()));
    SAFE_CONNECT(ui.actionUndo,  SIGNAL(triggered(bool)), this, SLOT(undo()));
    SAFE_CONNECT(ui.actionRedo,  SIGNAL(triggered(bool)), this, SLOT(redo()));

    SAFE_CONNECT(ui.actionAbout,   SIGNAL(triggered(bool)), this, SLOT(about()));
    SAFE_CONNECT(ui.actionAboutQt, SIGNAL(triggered(bool)), this, SLOT(aboutQt()));
}

MainWindow::~MainWindow() = default;

void MainWindow::loadFile()
{
    qDebug("loadFile()");

    auto filename = QFileDialog::getOpenFileName(this);
    QString statusBarMessage(this->tr("Could not load file"));

    QFile file(filename);
    if (true == file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        auto fileContent = QString::fromUtf8(file.readAll());
        this->Impl_->Ui_.textEdit->setPlainText(fileContent);
        this->Impl_->CurrentFilePath_ = filename;
        statusBarMessage = this->tr("File successfully loaded");
    }

    this->statusBar()->showMessage(statusBarMessage, this->Impl_->StatusBarMessageTimeoutMs_);
}

namespace
{
bool save_file(const QString& filename, const QTextEdit* textEdit)
{
    assert(false == filename.isEmpty());
    assert(nullptr != textEdit);

    QFile file(filename);
    auto r = file.open(QIODevice::WriteOnly|QIODevice::Text);
    if (true == r)
    {
        file.write(textEdit->toPlainText().toUtf8());
    }

    return r;
}
}

bool MainWindow::saveFile(const QString& filename)
{
    auto r = save_file(filename, this->Impl_->Ui_.textEdit);

    auto statusBarMessage = this->tr("Could not save file");
    if (true == r)
    {
        statusBarMessage = this->tr("File successfully saved");
        this->Impl_->CurrentFilePath_ = filename;
    }

    this->statusBar()->showMessage(statusBarMessage, this->Impl_->StatusBarMessageTimeoutMs_);
    return r;
}

bool MainWindow::saveFile()
{
    qDebug("saveFile()");

    if (true == this->Impl_->CurrentFilePath_.isEmpty())
    {
        return this->saveFileAs();
    }

    return this->saveFile(this->Impl_->CurrentFilePath_);
}

bool MainWindow::saveFileAs()
{
    qDebug("saveFileAs()");

    auto filename = QFileDialog::getSaveFileName(this);
    if (true == filename.isEmpty())
    {
        auto statusBarMessage = this->tr("Save operation aborted");
        this->statusBar()->showMessage(statusBarMessage, this->Impl_->StatusBarMessageTimeoutMs_);
        return false;
    }

    return this->saveFile(filename);
}

void MainWindow::copy()
{
    qDebug("copy()");
    this->Impl_->Ui_.textEdit->copy();
}

void MainWindow::undo()
{
    qDebug("undo()");
    this->Impl_->Ui_.textEdit->undo();
}

void MainWindow::redo()
{
    qDebug("redo()");
    this->Impl_->Ui_.textEdit->redo();
}

void MainWindow::about()
{
    qDebug("about()");

    auto title = this->tr("About %1").arg(this->windowTitle());
    auto text = this->tr("%1\nA sample Qt editor").arg(this->windowTitle());
    QMessageBox::about(this, title, text);
}

void MainWindow::aboutQt()
{
    qDebug("aboutQt()");
    QMessageBox::aboutQt(this, this->tr("About Qt"));
}

void MainWindow::cut()
{
    qDebug("cut()");
    this->Impl_->Ui_.textEdit->cut();
}

void MainWindow::paste()
{
    qDebug("paste()");
    this->Impl_->Ui_.textEdit->paste();
}

void MainWindow::loadNew()
{
    qDebug("loadNew()");

    if (false == this->mayDiscardDocument())
    {
        return;
    }

    this->Impl_->Ui_.textEdit->setPlainText("");
    this->Impl_->CurrentFilePath_.clear();
    auto statusBarMessage = this->tr("New file loaded");
    this->statusBar()->showMessage(statusBarMessage, this->Impl_->StatusBarMessageTimeoutMs_);
}

bool MainWindow::mayDiscardDocument()
{
    auto modified = this->Impl_->Ui_.textEdit->document()->isModified();
    if (true == modified)
    {
        auto title = this->tr("Unsaved changes");
        auto text = this->tr("You have unsaved changes in the current document. Do you want to save?");
        auto defaultButton = QMessageBox::StandardButton::Cancel;
        auto buttons = QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No | defaultButton;

        auto answer = QMessageBox::question(this, title, text, buttons, defaultButton);
        switch (answer)
        {
        case QMessageBox::StandardButton::Yes:
            return this->saveFile();

        case QMessageBox::StandardButton::No:
            return true;

        case QMessageBox::StandardButton::Abort:
            return false;

        default:
            Q_ASSERT(false);
            break;
        }
    }

    return false == modified;
}
