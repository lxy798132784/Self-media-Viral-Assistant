#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <QMainWindow>
#include <QString>
namespace mva::ui { class MainWindow : public QMainWindow { Q_OBJECT public: explicit MainWindow(QWidget *parent = nullptr); ~MainWindow(); void loadQml(const QString &qmlPath); }; }
#endif
