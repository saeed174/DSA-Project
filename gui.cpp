
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QFile>
#include <QStyle>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include "xmlCheck.h"
#include "minifier.h"
#include "compression_decompression.h"
#include "formatter.h"
// ==================== ShowGraph =====================
class Graph : public QWidget {
    QString filePath;

public:
    Graph(QString selectedPath, QWidget* parent = nullptr)
        : QWidget(parent), filePath(selectedPath)
    {
        setWindowTitle("Graph");
        resize(500, 400);

        // Main layout
        Main_layout = new QVBoxLayout();
        Main_layout->setSpacing(20);
        Main_layout->setContentsMargins(40, 40, 40, 40);
        setLayout(Main_layout);


        row1 = new QHBoxLayout();
        row1->setSpacing(20);

        fun1 = createStyledButton("Fun 1", "#ff6b6b");
        fun2 = createStyledButton("Fun 2", "#4ecdc4");

        row1->addWidget(fun1);
        row1->addWidget(fun2);

        row2 = new QHBoxLayout();
        row2->setSpacing(20);

        fun3 = createStyledButton("Fun 3", "#1a535c");
        fun4 = createStyledButton("Fun 4", "#ffa600", "black");

        row2->addWidget(fun3);
        row2->addWidget(fun4);


        Main_layout->addStretch();
        Main_layout->addLayout(row1);
        Main_layout->addLayout(row2);
        Main_layout->addStretch();

        applyShadowEffect(fun1);
        applyShadowEffect(fun2);
        applyShadowEffect(fun3);
        applyShadowEffect(fun4);
    }
    // signals
    // here

private:

    QPushButton* createStyledButton(QString text, QString color, QString textColor = "white")
    {
        QPushButton* btn = new QPushButton(text);

        QString style = QString(R"(
            QPushButton {
                background-color: %1;
                color: %2;
                font-weight: bold;
                border-radius: 12px;
                padding: 12px 20px;
                font-size: 16px;
            }
            QPushButton:hover {
                background-color: %1cc;  /* transparent hover */
            }
            QPushButton:pressed {
                background-color: %1aa;
                transform: scale(0.97);
            }
        )").arg(color, textColor);

        btn->setStyleSheet(style);
        btn->setFixedHeight(50);

        return btn;
    }
    void applyShadowEffect(QWidget* w)
    {
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(20);
        shadow->setColor(QColor(0, 0, 0, 90));
        shadow->setOffset(0, 4);
        w->setGraphicsEffect(shadow);
    }


    QVBoxLayout* Main_layout;
    QHBoxLayout* row1;
    QHBoxLayout* row2;
    QPushButton* fun1;
    QPushButton* fun2;
    QPushButton* fun3;
    QPushButton* fun4;

    //signal operation
    // here
};

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
    OperationsWindow(QString selectedPath, QWidget* parent = nullptr)
        : QWidget(parent), filePath(selectedPath)
    {
        setWindowTitle("XML Operations");
        resize(500, 380);

        // ================= MAIN PAGE LAYOUT ====================
        QVBoxLayout* page = new QVBoxLayout(this);
        page->setContentsMargins(25, 25, 25, 25);
        page->setSpacing(20);

        QLabel* header = new QLabel("XML OPERATIONS", this);
        header->setAlignment(Qt::AlignCenter);
        header->setStyleSheet("font-size: 23px; font-weight: bold; color: #333;");
        page->addWidget(header);

        QFrame* card = new QFrame(this);
        card->setStyleSheet(
            "QFrame {"
            " background: #ffffff;"
            " border-radius: 12px;"
            " padding: 20px;"
            " border: 1px solid #d0d0d0;"
            "}"
        );
        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(18);
        page->addWidget(card);


        consistency = createButton("Check Consistency", style()->standardIcon(QStyle::SP_DialogApplyButton), "#ff5252");
        Converting_to_JSON = createButton("Convert to JSON", style()->standardIcon(QStyle::SP_FileIcon), "#26a69a");
        Formatting = createButton("Format XML", style()->standardIcon(QStyle::SP_BrowserReload), "#1e88e5");
        Compressing = createButton("Compress File", style()->standardIcon(QStyle::SP_ArrowDown), "#fbc02d");
        Decompressing = createButton("Decompress File", style()->standardIcon(QStyle::SP_ArrowUp), "#6a1b9a");
        Minifying = createButton("Minify XML", style()->standardIcon(QStyle::SP_ComputerIcon), "#3949ab");

        QGridLayout* grid = new QGridLayout();
        grid->setHorizontalSpacing(20);
        grid->setVerticalSpacing(15);

        grid->addWidget(consistency, 0, 0);
        grid->addWidget(Converting_to_JSON, 0, 1);

        grid->addWidget(Formatting, 1, 0);
        grid->addWidget(Compressing, 1, 1);

        grid->addWidget(Decompressing, 2, 0);
        grid->addWidget(Minifying, 2, 1);

        cardLayout->addLayout(grid);

        //  SIGNALS
        connect(consistency, &QPushButton::clicked, this, &OperationsWindow::checkConsistency);
        connect(Minifying, &QPushButton::clicked, this, &OperationsWindow::runMinifying);
        connect(Compressing, &QPushButton::clicked, this, &OperationsWindow::runCompressing);
        connect(Decompressing, &QPushButton::clicked, this, &OperationsWindow::runDecompressing);
        connect(Formatting, &QPushButton::clicked, this, &OperationsWindow::runFormatting);

        // (optional) connect JSON later
    }

private:
    QString filePath;

    QPushButton* consistency;
    QPushButton* Converting_to_JSON;
    QPushButton* Formatting;
    QPushButton* Compressing;
    QPushButton* Decompressing;
    QPushButton* Minifying;


    //  UNIVERSAL BUTTON CREATOR (with icon + color)
    QPushButton* createButton(QString text, QIcon icon, QString colorHex)
    {
        QPushButton* btn = new QPushButton(icon, text);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setIconSize(QSize(22, 22));

        btn->setStyleSheet(
            "QPushButton {"
            " background: " + colorHex + ";"
            " color: white;"
            " padding: 10px;"
            " border-radius: 8px;"
            " font-size: 14px;"
            " font-weight: bold;"
            " text-align: left;"
            "}"
            "QPushButton:hover {"
            " background: " + colorHex + "cc;"
            "}"
        );
        return btn;
    }

private slots:


    void checkConsistency()
    {
        string xml = readXMLFile_withSpaces(filePath.toStdString());
        bool ok = checkXMLConsistency(xml);

        if (ok)
            QMessageBox::information(this, "XML Check", "XML is CONSISTENT ✔");
        else
            QMessageBox::warning(this, "XML Check", "XML has ERRORS ❌");
    }


    void runMinifying()
    {
        string xml = readXMLFile_withSpaces(filePath.toStdString());

        XMLMinifier minifier;
        string minified = minifier.minifyXML(xml);

        ShowFileWindow* viewer =
            new ShowFileWindow(QString::fromStdString(minified), nullptr, false);

        viewer->show();
    }


    void runCompressing()
    {
        QString output = QFileDialog::getSaveFileName(
            this, "Save Compressed File", "", "Binary Files (*.bin)");

        if (output.isEmpty()) return;

        compressor c;
        c.compress_file(filePath.toStdString(), output.toStdString());

        QMessageBox::information(this, "Done", "File Compressed Successfully!");
    }


    void runDecompressing()
    {
        QString output = QFileDialog::getSaveFileName(
            this, "Save Decompressed File", "", "XML Files (*.xml)");

        if (output.isEmpty()) return;

        decompressor d;
        d.decompress_file(filePath.toStdString(), output.toStdString());

        QMessageBox::information(this, "Done", "File Decompressed Successfully!");
    }


    void runFormatting()
    {
        string xml = readXMLFile_withSpaces(filePath.toStdString());

        XMLFormatter formatter;
        string formatted = formatter.formatXML(xml);

        ShowFileWindow* viewer =
            new ShowFileWindow(QString::fromStdString(formatted), nullptr, false);
        viewer->show();
    }
};


// ==================== MAIN WINDOW =====================
class Window : public QWidget {
public:
    Window(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowTitle("XML Project");
        resize(480, 360);

        // ===================== Main Layout =========================
        QVBoxLayout* page = new QVBoxLayout(this);
        page->setContentsMargins(30, 30, 30, 30);
        page->setSpacing(20);


        QLabel* title = new QLabel("XML PROJECT", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-size: 26px; font-weight: bold; color: #333;");
        page->addWidget(title);

        //  Card Container
        QFrame* card = new QFrame(this);
        card->setStyleSheet(
            "QFrame {"
            " background: #ffffff;"
            " border-radius: 15px;"
            " padding: 20px;"
            " border: 1px solid #ddd;"
            "}"
        );

        //white box
        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(20);

        //  Select File Button
        QPushButton* selectFile = new QPushButton("Select XML File");
        stylePrimaryButton(selectFile);
        cardLayout->addWidget(selectFile);

        //  3 buttons
        QHBoxLayout* actions = new QHBoxLayout();
        actions->setSpacing(15);

        QPushButton* operationsBtn = new QPushButton("Operations");
        QPushButton* graphBtn = new QPushButton("Show Graph");
        QPushButton* showFileBtn = new QPushButton("Show File");
        // design for 3 button
        styleSecondaryButton(operationsBtn, "#673AB7");
        styleSecondaryButton(graphBtn, "#F57C00");
        styleSecondaryButton(showFileBtn, "#009688");

        actions->addWidget(operationsBtn);
        actions->addWidget(graphBtn);
        actions->addWidget(showFileBtn);

        cardLayout->addLayout(actions);

        // Add Card to page
        page->addWidget(card);

        //  CONNECT SIGNALS
        connect(selectFile, &QPushButton::clicked,
                this, &Window::openFileDialog);

        connect(operationsBtn, &QPushButton::clicked,
                this, &Window::processXML);

        connect(showFileBtn, &QPushButton::clicked,
                this, &Window::showSelectedFile);

        connect(graphBtn, &QPushButton::clicked,
                this, &Window::openGraph);
    }

private:



    void stylePrimaryButton(QPushButton* btn) {
        btn->setStyleSheet(
            "QPushButton {"
            "  background: #2196F3;"
            "  color: white;"
            "  border-radius: 10px;"
            "  padding: 12px;"
            "  font-size: 16px;"
            "}"
            "QPushButton:hover { background: #42A5F5; }"
            "QPushButton:pressed { background: #1E88E5; }"
        );
        btn->setCursor(Qt::PointingHandCursor);
    }

    void styleSecondaryButton(QPushButton* btn, const QString& color) {
        btn->setStyleSheet(
            "QPushButton {"
            "  background: " + color + ";"
            "  color: white;"
            "  border-radius: 8px;"
            "  padding: 10px;"
            "  font-size: 14px;"
            "}"
            "QPushButton:hover { opacity: 0.85; }"
        );
        btn->setCursor(Qt::PointingHandCursor);
    }



    QString selectedFilePath;
    OperationsWindow* opWindow = nullptr;
    Graph* graph = nullptr;

    void openGraph() {
        if (selectedFilePath.isEmpty()) {
            QMessageBox::warning(this, "Error", "Select a file first.");
            return;
        }
        if (!graph)
            graph = new Graph(selectedFilePath);
        graph->show();
        graph->raise();
    }

    void openFileDialog() {
        QString filePath = QFileDialog::getOpenFileName(
            this, "Select XML File", "", "XML Files (*.xml)"
        );

        if (!filePath.isEmpty()) {
            selectedFilePath = filePath;
            QMessageBox::information(this, "File Selected", selectedFilePath);
        }
    }

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
