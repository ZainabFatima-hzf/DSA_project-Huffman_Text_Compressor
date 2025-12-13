#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../src/core/Compressor.h"  // ADD THIS LINE

// Forward declarations
class Compressor;
class HuffmanNode;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectFileButton_clicked();
    void on_compressButton_clicked();
    void on_decompressButton_clicked();
    void on_showTreeButton_clicked();

private:
    Ui::MainWindow *ui;
    Compressor lastCompressor;  // Store compressor for tree visualization
};

#endif // MAINWINDOW_H
