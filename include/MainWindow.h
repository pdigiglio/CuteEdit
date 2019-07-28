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
private:
    struct impl;
    std::unique_ptr<impl> Impl_;

    using base = QMainWindow;
};
