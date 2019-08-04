#include "MainWindow.h"

#include "qt_utils.h"
#include "ui_MainWindow.h"

#include "SettingGroup.h"
#include "Settings.h"
#include "TemplateHandler.h"

#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QSize>
#include <QString>

#include <cassert>
#include <memory>
#include <numeric>

struct MainWindow::impl
{
    impl()
        :
          Ui_(),
          CurrentFilePath_(),
          StatusBarMessageTimeoutMs_(3000),
          StatLabel_(nullptr)
    {}

    Ui::MainWindow Ui_;
    QString CurrentFilePath_;
    int StatusBarMessageTimeoutMs_;
    QLabel* StatLabel_;
};

MainWindow::MainWindow(QWidget *parent)
    :
      base(parent),
      Impl_(std::make_unique<impl>())
{
    this->Impl_->Ui_.setupUi(this);

    // -- Ui --
    this->setWindowTitle(this->tr("CuteEdit"));

    this->Impl_->StatLabel_ = new QLabel;
    this->statusBar()->addPermanentWidget(this->Impl_->StatLabel_);

    auto* toolbarMenu = this->Impl_->Ui_.menu_Settings->addMenu(this->tr("&Toolbars"));
    toolbarMenu->setToolTip(this->tr("Show/hide toolbar"));
    auto* toggleToolbarViewAction = this->Impl_->Ui_.ToolBar_->toggleViewAction();

    toolbarMenu->addAction(toggleToolbarViewAction);

    new TemplateHandler(this->Impl_->Ui_.ListView_, this->Impl_->Ui_.textEdit, this->Impl_->Ui_.TemplateDocker_);

    // -- Connections --
    this->setConnections();

    // -- Read the settings --
    this->readSettings();
}

void MainWindow::setConnections()
{
    auto& ui = this->Impl_->Ui_;
    SAFE_CONNECT(ui.textEdit, SIGNAL(textChanged()), this, SLOT(updateStats()));

    SAFE_CONNECT(ui.actionNew,    SIGNAL(triggered(bool)), this, SLOT(loadNew()));
    SAFE_CONNECT(ui.actionOpen,   SIGNAL(triggered(bool)), this, SLOT(loadFile()));
    SAFE_CONNECT(ui.actionSave,   SIGNAL(triggered(bool)), this, SLOT(saveFile()));
    SAFE_CONNECT(ui.actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(saveFileAs()));

    SAFE_CONNECT(ui.textEdit, SIGNAL(copyAvailable(bool)), ui.actionCopy, SLOT(setEnabled(bool)));
    SAFE_CONNECT(ui.textEdit, SIGNAL(copyAvailable(bool)), ui.actionCut,  SLOT(setEnabled(bool)));
    SAFE_CONNECT(ui.textEdit, &QTextEdit::copyAvailable, [](bool available) { qDebug("copyAvailable(available=%d)", available); });

    SAFE_CONNECT(ui.textEdit, SIGNAL(undoAvailable(bool)), ui.actionUndo, SLOT(setEnabled(bool)));
    SAFE_CONNECT(ui.textEdit, &QTextEdit::undoAvailable, [](bool available) { qDebug("undoAvailable(available=%d)", available); });

    SAFE_CONNECT(ui.textEdit, SIGNAL(redoAvailable(bool)), ui.actionRedo, SLOT(setEnabled(bool)));
    SAFE_CONNECT(ui.textEdit, &QTextEdit::redoAvailable, [](bool available) { qDebug("redoAvailable(available=%d)", available); });

    SAFE_CONNECT(ui.actionCut,   SIGNAL(triggered(bool)), this, SLOT(cut()));
    SAFE_CONNECT(ui.actionPaste, SIGNAL(triggered(bool)), this, SLOT(paste()));
    SAFE_CONNECT(ui.actionCopy,  SIGNAL(triggered(bool)), this, SLOT(copy()));
    SAFE_CONNECT(ui.actionUndo,  SIGNAL(triggered(bool)), this, SLOT(undo()));
    SAFE_CONNECT(ui.actionRedo,  SIGNAL(triggered(bool)), this, SLOT(redo()));

    SAFE_CONNECT(ui.actionAbout,   SIGNAL(triggered(bool)), this, SLOT(about()));
    SAFE_CONNECT(ui.actionAboutQt, SIGNAL(triggered(bool)), this, SLOT(aboutQt()));
}

MainWindow::~MainWindow()
{
    this->writeSettings();
}

void MainWindow::loadFile(const QString &filename, const QString &filecontent)
{
    this->Impl_->Ui_.textEdit->setPlainText(filecontent);
    this->Impl_->CurrentFilePath_ = filename;

    this->Impl_->Ui_.textEdit->undoAvailable(false);
    this->Impl_->Ui_.textEdit->redoAvailable(false);
    this->Impl_->Ui_.textEdit->copyAvailable(false);

    this->updateStats();
}

void MainWindow::loadFile()
{
    qDebug("loadFile()");

    auto filename = QFileDialog::getOpenFileName(this);
    QString statusBarMessage(this->tr("Could not load file"));

    QFile file(filename);
    if (true == file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        auto fileContent = QString::fromUtf8(file.readAll());
        this->loadFile(filename, fileContent);
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

    this->loadFile("", "");
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

void MainWindow::updateStats()
{
    auto content = this->Impl_->Ui_.textEdit->document()->toPlainText();
    content = content.simplified();

    // TODO: improve this!
    auto words = content.split(" ");

    auto wordCount = words.count();
    if (1 == wordCount && true == words[0].isEmpty())
    {
        wordCount = 0;
    }

    auto charCount = std::accumulate(std::cbegin(words), std::cend(words), 0,
                    [](unsigned lhs, const QString& rhs) { return lhs + static_cast<unsigned>(rhs.length()); });

    auto statsString = this->tr("Chars: %1; Words: %2").arg(charCount).arg(wordCount);
    this->Impl_->StatLabel_->setText(statsString);
}

void MainWindow::readSettings()
{
    qDebug("readSettings()");

    auto size = setting::get("Size", setting::SettingGroup::MainWindow, this->sizeHint());
    qDebug("size: width=%d, height=%d", size.width(), size.height());
    this->resize(size);

    auto properties = setting::get<QByteArray>("Properties", setting::SettingGroup::MainWindow);
    this->restoreState(properties);
}

void MainWindow::writeSettings()
{
    qDebug("writeSettings()");

    setting::set("Size", setting::SettingGroup::MainWindow, this->size());
    setting::set("Properties", setting::SettingGroup::MainWindow, this->saveState());
}
