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

private:
  Ui::MainWindow *ui;
public slots:
  void process_text() const;
  void start_machine() const;
  void step_machine() const;
  void back_step_machine() const;
  void show_log() const;
  void create_machine(const QString program);
  void set_current_machine(const QString name) const;

  void about_clicked();

  void debug_message(const QString s) const;

  void machine_rename_handler();
};

#endif // MAINWINDOW_H
