#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[]) {
  qSetMessagePattern("[%{type}] %{function}: %{message}");
  QApplication a(argc, argv);
  a.setDesktopFileName("acoustiqt");
  MainWindow w;
  w.show();
  return QCoreApplication::exec();
}
