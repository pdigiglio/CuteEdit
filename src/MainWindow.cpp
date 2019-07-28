#include "MainWindow.h"

#include "ui_MainWindow.h"

#include <memory>

struct MainWindow::impl
{
    impl()
        :
          Ui_()
    {}

    Ui::MainWindow Ui_;
};

MainWindow::MainWindow(QWidget *parent)
    :
      base(parent),
      Impl_(std::make_unique<impl>())
{
    this->Impl_->Ui_.setupUi(this);
}

MainWindow::~MainWindow() = default;
