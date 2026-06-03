#include "ui/main_window.h"
namespace mva::ui {
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {}
MainWindow::~MainWindow() = default;
void MainWindow::loadQml(const QString &qmlPath) { Q_UNUSED(qmlPath); }
} // namespace mva::ui

cat > /workspace/include/ui/main_window.h << 'EOF'
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <QMainWindow>
#include <QString>
namespace mva::ui {
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadQml(const QString &qmlPath);
};
} // namespace mva::ui
#endif
