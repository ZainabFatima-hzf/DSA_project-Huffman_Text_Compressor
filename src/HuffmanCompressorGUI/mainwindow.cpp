#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "TreeVisualizer.h"
#include "../src/core/Compressor.h"
#include "../src/core/Decompressor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectFileButton_clicked()
{
    QString filter = "Text Files (*.txt);;Huffman Compressed Files (*.huf);;All Files (*.*)";

    QString filePath = QFileDialog::getOpenFileName(
        this, tr("Select File to Process"), QDir::homePath(), filter
        );

    if (!filePath.isEmpty()) {
        ui->lineEdit_filePath->setText(filePath);
        ui->textEdit_status->setText(tr("File selected: ") + filePath);
        ui->label_originalSize->setText("Original Size: -");
        ui->label_compressedSize->setText("Compressed Size: -");
    } else {
        ui->textEdit_status->setText(tr("File selection cancelled."));
    }
}


void MainWindow::on_showTreeButton_clicked()
{
    qDebug() << "=== DEBUG: Show Tree Button Clicked ===";

    // 2. Try to get root
    qDebug() << "Calling getTreeRoot()...";
    HuffmanNode* root = nullptr;
    try {
        root = lastCompressor.getTreeRoot();
    } catch (...) {
        qDebug() << "CRASH: Exception in getTreeRoot()";
        return;
    }
    qDebug() << "Got root pointer:" << root;

    // 3. Check root
    if (!root) {
        qDebug() << "Root is null - tree not built";
        QMessageBox::information(this, "Info", "Please compress a file first");
        return;
    }
    qDebug() << "Root is not null";

    // 4. Try to access root data
    qDebug() << "Attempting to access root data...";
    try {
        char ch = root->character;
        int freq = root->frequency;
        qDebug() << "Root data - char:" << ch << "freq:" << freq;
    } catch (...) {
        qDebug() << "CRASH: Cannot access root data - dangling pointer!";
        QMessageBox::critical(this, "Error", "Tree data corrupted");
        return;
    }

    // 5. Create TreeVisualizer
    qDebug() << "Creating TreeVisualizer...";
    try {
        TreeVisualizer* visualizer = new TreeVisualizer(root, this);
        visualizer->setAttribute(Qt::WA_DeleteOnClose);
        visualizer->show();
        qDebug() << "TreeVisualizer created successfully";
    } catch (...) {
        qDebug() << "CRASH: Exception in TreeVisualizer constructor";
    }
}

void MainWindow::on_compressButton_clicked()
{
    QString inputPath = ui->lineEdit_filePath->text();

    if (inputPath.isEmpty() || !inputPath.endsWith(".txt", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please select a valid .txt file for compression."));
        ui->textEdit_status->setText(tr("ERROR: Invalid input file selected."));
        return;
    }

    ui->textEdit_status->setText(tr("Starting compression..."));

    QFileInfo inputFileInfo(inputPath);
    qint64 originalSize = inputFileInfo.size();

    QString outputPath = inputPath;
    outputPath.replace(".txt", ".huf", Qt::CaseInsensitive);

    // FIX: Convert QString to std::string
    bool success = lastCompressor.compressFile(
        inputPath.toStdString(),
        outputPath.toStdString()
        );

    if (success) {
        QFileInfo outputFileInfo(outputPath);
        qint64 compressedSize = outputFileInfo.size();

        ui->textEdit_status->setText(tr("Compression successful! Output saved to: ") + outputPath);
        ui->label_originalSize->setText(tr("Original Size: ") + QString::number(originalSize) + tr(" bytes"));
        ui->label_compressedSize->setText(tr("Compressed Size: ") + QString::number(compressedSize) + tr(" bytes"));

        if (originalSize > 0 && compressedSize > 0) {
            double ratio = (double)originalSize / compressedSize;
            ui->textEdit_status->append(tr("Compression Ratio: ") + QString::number(ratio, 'f', 2) + tr(":1"));
        }

    } else {
        QMessageBox::critical(this, tr("Compression Failed"), tr("The core compression process failed."));
        ui->textEdit_status->setText(tr("Compression FAILED. Check core logic for errors."));
    }
}


void MainWindow::on_decompressButton_clicked()
{
    QString inputPath = ui->lineEdit_filePath->text();

    if (inputPath.isEmpty() || !inputPath.endsWith(".huf", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please select a valid .huf file for decompression."));
        ui->textEdit_status->setText(tr("ERROR: Invalid input file selected."));
        return;
    }

    ui->textEdit_status->setText(tr("Starting decompression..."));

    QString outputPath = inputPath;
    outputPath.replace(".huf", "_decompressed.txt", Qt::CaseInsensitive);

    // FIX: Add Decompressor include and convert QString to std::string
    Decompressor decompressor;
    bool success = decompressor.decompressFile(
        inputPath.toStdString(),
        outputPath.toStdString()
        );

    if (success) {
        ui->textEdit_status->setText(tr("Decompression successful! Output saved to: ") + outputPath);
        ui->label_originalSize->setText("Original Size: -");
        ui->label_compressedSize->setText("Compressed Size: -");
    } else {
        QMessageBox::critical(this, tr("Decompression Failed"), tr("The core decompression process failed."));
        ui->textEdit_status->setText(tr("Decompression FAILED. Check core logic for errors."));
    }
}
