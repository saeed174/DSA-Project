#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QFile>

// Include your XML operations here
#include "xmlCheck.h"
#include "formatter.h"

// ==================== SHOW FILE WINDOW =====================
class ShowFileWindow : public QWidget {
public:
    ShowFileWindow(QString content, QWidget* parent = nullptr, bool isRawFile = true)
        : QWidget(parent)
    {
        setWindowTitle("File Viewer");
        resize(500, 400);

        QVBoxLayout* layout = new QVBoxLayout(this);
        QTextEdit* textArea = new QTextEdit(this);
        textArea->setReadOnly(true);

        // If reading from file
        if (isRawFile) {
            QFile file(content);
            if (file.open(QFile::ReadOnly | QFile::Text)) {
                QString fileData = file.readAll();
                textArea->setText(fileData);
            }
        }
        else {
            textArea->setText(content); // Show processed XML
        }

        layout->addWidget(textArea);
    }
};

// ==================== OPERATION WINDOW =====================
class OperationsWindow : public QWidget {
public:
    QString filePath;

    OperationsWindow(QString selectedPath, QWidget* parent = nullptr)
        : QWidget(parent), filePath(selectedPath)
    {
        setWindowTitle("Operations Window");
        resize(400, 300);

        main_layout_op = new QVBoxLayout();
        setLayout(main_layout_op);

        row1_op = new QHBoxLayout();
        consistency = new QPushButton("consistency");
        Converting_to_JSON = new QPushButton("Converting_to_JSON");
        row1_op->addWidget(Converting_to_JSON);
        row1_op->addWidget(consistency);

        row2_op = new QHBoxLayout();
        Formatting = new QPushButton("Formatting");
        Compressing = new QPushButton("Compressing");
        row2_op->addWidget(Formatting);
        row2_op->addWidget(Compressing);

        row3_op = new QHBoxLayout();
        Decompressing = new QPushButton("Decompressing");
        Minifying = new QPushButton("Minifying");
        row3_op->addWidget(Decompressing);
        row3_op->addWidget(Minifying);

        main_layout_op->addLayout(row1_op);
        main_layout_op->addLayout(row2_op);
        main_layout_op->addLayout(row3_op);

        // Design
        consistency->setStyleSheet("background-color: #ff6b6b; color: white; font-weight: bold;border-radius: 8px;padding: 10px;font-size: 15px;");
        Converting_to_JSON->setStyleSheet("background-color: #4ecdc4; color: white; font-weight: bold;border-radius: 8px;padding: 10px;font-size: 15px;");
        Formatting->setStyleSheet("background-color: #1a535c; color: white; font-weight: bold;border-radius: 8px;padding: 10px;font-size: 15px;");
        Compressing->setStyleSheet("background-color: #ffa600; color: black; font-weight: bold;border-radius: 8px;padding: 10px;font-size: 15px;");
        Decompressing->setStyleSheet("background-color: #6a4c93; color: white; font-weight: bold;border-radius: 8px;padding: 10px;font-size: 15px;");
        Minifying->setStyleSheet("background-color: #3d5af1; color: white; font-weight: bold;border-radius: 8px;padding: 10px;font-size: 15px;");

        // ===== CONNECT BUTTONS =====
        connect(consistency, &QPushButton::clicked, this, &OperationsWindow::checkConsistency);



    }

private:
    QVBoxLayout* main_layout_op;
    QHBoxLayout* row1_op;
    QHBoxLayout* row2_op;
    QHBoxLayout* row3_op;
    QPushButton* consistency;
    QPushButton* Converting_to_JSON;
    QPushButton* Formatting;
    QPushButton* Compressing;
    QPushButton* Decompressing;
    QPushButton* Minifying;

private slots:

    /// ================= CONSISTENCY CHECK =================
    void checkConsistency() {
        string xml = readXMLFile_withSpaces(filePath.toStdString());
        bool ok = checkXMLConsistency(xml);
        if (ok)
            QMessageBox::information(this, "XML Check", "XML is CONSISTENT ✔");
        else
            QMessageBox::warning(this, "XML Check", "XML has ERRORS ❌");
    }





};

// ==================== MAIN WINDOW =====================
class Window : public QWidget {
public:
    Window(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowTitle("Data_Structure");
        resize(400, 300);

        main_layout = new QVBoxLayout();
        setLayout(main_layout);

        row0 = new QLabel("XML_PROJECT");
        row0->setAlignment(Qt::AlignCenter);
        main_layout->addWidget(row0);

        row1 = new QHBoxLayout();
        select_XML_File = new QPushButton("Select File");
        row1->addWidget(select_XML_File);

        row2 = new QHBoxLayout();
        Operations = new QPushButton("XML_Operations");
        ShowGraph = new QPushButton("Show Graph");
        showFile = new QPushButton("Show File");

        row2->addWidget(Operations);
        row2->addWidget(ShowGraph);
        row2->addWidget(showFile);

        main_layout->addLayout(row1);
        main_layout->addLayout(row2);

        //=========================design=================================
        select_XML_File->setStyleSheet(
        "QPushButton {"
         "background-color: #2196F3;"
         "color: white;"
         "border-radius: 8px;"
         "padding: 10px;"
         "font-size: 15px;""}"
         "QPushButton:hover {");
        Operations->setStyleSheet(
            "QPushButton {"
            "background-color: #9C27B0;"
            "color: white;"
            "border-radius: 8px;"
            "padding: 10px;"
            "font-size: 15px;"
            "}"
        );
        ShowGraph->setStyleSheet(
            "QPushButton {"
            "background-color: #FF9800;"
            "color: white;"
            "border-radius: 8px;"
            "padding: 10px;"
            "font-size: 15px;"
            "}"
        );
        showFile->setStyleSheet(
        "QPushButton {"
         "background-color: #9C27B0;"
         "color: white;"
         "border-radius: 8px;"
         "padding: 10px;"
         "font-size: 15px;""}"
         "QPushButton:hover {");
        //=========================design=================================

        // Connect buttons
        connect(select_XML_File, &QPushButton::clicked,
                this, &Window::openFileDialog);

        connect(Operations, &QPushButton::clicked,
                this, &Window::processXML);

        connect(showFile, &QPushButton::clicked,
                this, &Window::showSelectedFile);
    }

private:
    QVBoxLayout* main_layout;
    QLabel* row0;
    QHBoxLayout* row1;
    QHBoxLayout* row2;
    QPushButton* select_XML_File;
    QPushButton* ShowGraph;
    QPushButton* Operations;
    QPushButton* showFile;

    QString selectedFilePath;
    OperationsWindow* opWindow = nullptr;

    // ===== SELECT FILE =====
    void openFileDialog() {
        QString filePath =
            QFileDialog::getOpenFileName(this,"Select XML File","","XML Files (*.xml)");

        if (!filePath.isEmpty()) {
            selectedFilePath = filePath;
            QMessageBox::information(this, "File Selected", selectedFilePath);
        }
    }

    // ===== OPEN OPERATIONS WINDOW =====
    void processXML() {
        if (selectedFilePath.isEmpty()) {
            QMessageBox::warning(this, "Error", "Select a file first.");
            return;
        }

        if (!opWindow)
            opWindow = new OperationsWindow(selectedFilePath);

        opWindow->show();
        opWindow->raise();
    }

    // ===== SHOW RAW XML FILE =====
    void showSelectedFile() {
        if (selectedFilePath.isEmpty()) {
            QMessageBox::warning(this, "Error", "Select a file first.");
            return;
        }
        ShowFileWindow* viewer = new ShowFileWindow(selectedFilePath);
        viewer->show();
    }
};

// ==================== MAIN() =====================
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}
