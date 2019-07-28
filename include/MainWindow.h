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
    void loadNew();

private slots:
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

    /// @brief Load a file with a given content.
    /// @param filename The file name.
    /// @param filecontent The file content.
    void loadFile(const QString& filename, const QString& filecontent);

    using base = QMainWindow;
};
