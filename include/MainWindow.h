#pragma once

#include <QMainWindow>

#include <memory>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
public slots:

private slots:
    void loadNew();
    void loadFile();
    bool saveFile();
    bool saveFileAs();

    void copy();
    void cut();
    void paste();
    void undo();
    void redo();

    void about();
    void aboutQt();

private:
    struct impl;
    std::unique_ptr<impl> Impl_;

    /// @brief Save a file.
    /// @param filename The target file name.
    /// @return \c true, if file successful saved; \c false, otherwise.
    bool saveFile(const QString& filename);

    bool mayDiscardDocument();

    using base = QMainWindow;
};
