#include <QFileDialog> // Required for the file selection dialog
#include <QDir>        // Required for accessing system paths (like home directory)
#include <QMessageBox> // Recommended for displaying errors
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo> // For getting file size
#include "../core/Compressor.h" // **MUST** include Member 2's class
#include "../utils/FileIO.h" // Might be needed for size info
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
    // Define filters to show relevant file types
    QString filter = "Text Files (*.txt);;Huffman Compressed Files (*.huf);;All Files (*.*)";

    // Open the system file dialog
    QString filePath = QFileDialog::getOpenFileName(
        this,                 // Parent window
        tr("Select File to Process"), // Dialog title
        QDir::homePath(),     // Starting directory
        filter                // File filters
        );

    if (!filePath.isEmpty()) {
        // Update the QLineEdit and status display
        ui->lineEdit_filePath->setText(filePath);
        ui->textEdit_status->setText(tr("File selected: ") + filePath);

        // Clear previous results display
        ui->label_originalSize->setText("Original Size: -");
        ui->label_compressedSize->setText("Compressed Size: -");
    } else {
        // User cancelled the dialog
        ui->textEdit_status->setText(tr("File selection cancelled."));
    }
}


void MainWindow::on_showTreeButton_clicked()
{

}


void MainWindow::on_compressButton_clicked()
{
    QString inputPath = ui->lineEdit_filePath->text();

    // 1. Input Validation: Check for .txt file
    if (inputPath.isEmpty() || !inputPath.endsWith(".txt", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please select a valid .txt file for compression."));
        ui->textEdit_status->setText(tr("ERROR: Invalid input file selected."));
        return;
    }

    ui->textEdit_status->setText(tr("Starting compression..."));

    QFileInfo inputFileInfo(inputPath);
    qint64 originalSize = inputFileInfo.size();

    // 2. Define Output Path
    QString outputPath = inputPath;
    // Replaces .txt with .huf for the output filename
    outputPath.replace(".txt", ".huf", Qt::CaseInsensitive);

    // 3. CALL CORE COMPRESSOR LOGIC
    Compressor compressor;
    bool success = compressor.compressFile(inputPath, outputPath);

    if (success) {
        QFileInfo outputFileInfo(outputPath);
        qint64 compressedSize = outputFileInfo.size();

        // 4. Update the UI with results
        ui->textEdit_status->setText(tr("Compression successful! Output saved to: ") + outputPath);

        // Display Sizes
        ui->label_originalSize->setText(tr("Original Size: ") + QString::number(originalSize) + tr(" bytes"));
        ui->label_compressedSize->setText(tr("Compressed Size: ") + QString::number(compressedSize) + tr(" bytes"));

        // Calculate and display Ratio (for presentation)
        if (compressedSize > 0) {
            double ratio = (double)originalSize / compressedSize;
            ui->textEdit_status->append(tr("Compression Ratio (Original/Compressed): ") + QString::number(ratio, 'f', 2) + tr(":1"));
        }

    } else {
        // Error handling (assuming Compressor handles error messages)
        QMessageBox::critical(this, tr("Compression Failed"), tr("The core compression process failed."));
        ui->textEdit_status->setText(tr("Compression FAILED. Check core logic for errors."));
    }
}


void MainWindow::on_decompressButton_clicked()
{
    QString inputPath = ui->lineEdit_filePath->text();

    // 1. Input Validation: Check for .huf file
    if (inputPath.isEmpty() || !inputPath.endsWith(".huf", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please select a valid .huf file for decompression."));
        ui->textEdit_status->setText(tr("ERROR: Invalid input file selected."));
        return;
    }

    ui->textEdit_status->setText(tr("Starting decompression..."));

    // 2. Define Output Path (e.g., file.huf -> file_decompressed.txt)
    QString outputPath = inputPath;
    outputPath.replace(".huf", "_decompressed.txt", Qt::CaseInsensitive);

    // 3. CALL CORE DECOMPRESSOR LOGIC
    Decompressor decompressor;
    bool success = decompressor.decompressFile(inputPath, outputPath);

    if (success) {
        // 4. Update the UI
        ui->textEdit_status->setText(tr("Decompression successful! Original text saved to: ") + outputPath);

        // Clear size labels as decompression doesn't track size reduction
        ui->label_originalSize->setText("Original Size: -");
        ui->label_compressedSize->setText("Compressed Size: -");

    } else {
        QMessageBox::critical(this, tr("Decompression Failed"), tr("The core decompression process failed."));
        ui->textEdit_status->setText(tr("Decompression FAILED. Check core logic for errors."));
    }
}

