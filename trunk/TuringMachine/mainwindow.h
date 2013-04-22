#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "turingmachine.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  //TMExecution tmexec;

private:
  Ui::MainWindow *ui;
public slots:
  void process_text();
  void start_machine();
  void step_machine();
  void back_step_machine();
  void show_log();

  void about_clicked();

  void set_current_machine(QString name);
};

#endif // MAINWINDOW_H
