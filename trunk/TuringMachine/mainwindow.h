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

  void readSettings();
  void writeSettings();

  void closeEvent(QCloseEvent *);

public slots:
  void process_text() const;
  void step_machine() const;
  void back_step_machine() const;
  void show_log() const;
  QString create_machine(const QString program);
  void set_current_machine(const QString name);

  void about_clicked();

  void debug_message(const QString s) const;

  void machine_rename_handler();

  void save_machine();
  void load_machine();

  void add_machine();
  void remove_machine();

  void show_current_machine_code();
};

#endif // MAINWINDOW_H
