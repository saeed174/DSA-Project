#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QFile>
#include <QSplitter>
#include <QStyle>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include "xmlCheck.h"
#include "minifier.h"
#include "compression_decompression.h"
#include "formatter.h"
# include "SimpleXmlToJson.h"

// ==================== Write xml =====================
class WriteXmlWindow : public QWidget {
public:
    explicit WriteXmlWindow(std::function<void(QString)> onXmlReady)
        : QWidget(nullptr), xmlReadyCallback(onXmlReady)
    {
        setWindowTitle("Write XML");
        resize(650, 500);

        setWindowFlag(Qt::Window);
        setAttribute(Qt::WA_DeleteOnClose);

        this->setStyleSheet("background:#020617;");

        // ================= MAIN LAYOUT =================
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(25, 25, 25, 25);
        layout->setSpacing(18);

        // ================= TITLE =================
        QLabel* title = new QLabel("WRITE XML", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(
            "font-size: 22px;"
            "font-weight: 800;"
            "letter-spacing: 2px;"
            "color: #e5e7eb;"
        );
        layout->addWidget(title);

        // ================= CARD =================
        QFrame* card = new QFrame(this);
        card->setStyleSheet(
            "QFrame {"
            " background: #0f172a;"
            " border-radius: 16px;"
            " padding: 20px;"
            "}"
        );

        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(16);

        // ================= EDITOR =================
        editor = new QTextEdit(this);
        editor->setPlaceholderText(
            "<root>\n"
            "    <element>value</element>\n"
            "</root>"
        );
        editor->setStyleSheet(
            "QTextEdit {"
            " background:#020617;"
            " color:#e5e7eb;"
            " border:1px solid #334155;"
            " border-radius:12px;"
            " font-family: Consolas;"
            " font-size:14px;"
            " padding:12px;"
            "}"
        );

        cardLayout->addWidget(editor);

        // ================= BUTTONS =================
        QHBoxLayout* buttons = new QHBoxLayout();
        buttons->setSpacing(15);

        QPushButton* saveBtn = new QPushButton("Save XML");
        QPushButton* useBtn  = new QPushButton("Use For Operations");

        styleButton(saveBtn, "#22c55e");   // green
        styleButton(useBtn,  "#3b82f6");   // blue

        buttons->addWidget(saveBtn);
        buttons->addWidget(useBtn);

        cardLayout->addLayout(buttons);
        layout->addWidget(card);

        // ================= SIGNALS =================
        connect(saveBtn, &QPushButton::clicked, this, &WriteXmlWindow::saveToFile);
        connect(useBtn,  &QPushButton::clicked, this, &WriteXmlWindow::useForOperations);
    }

private:
    QTextEdit* editor;
    std::function<void(QString)> xmlReadyCallback;

    // ================= STYLE BUTTON =================
    void styleButton(QPushButton* btn, const QString& color)
    {
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(
            "QPushButton {"
            " background:" + color + ";"
            " color:white;"
            " border-radius:10px;"
            " padding:12px;"
            " font-size:14px;"
            " font-weight:600;"
            "}"
            "QPushButton:hover { background:" + color + "cc; }"
        );
    }

    // ================= SAVE =================
    void saveToFile()
    {
        QString path = QFileDialog::getSaveFileName(
            this, "Save XML", "", "XML Files (*.xml)"
        );

        if (path.isEmpty())
            return;

        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Cannot save file");
            return;
        }

        file.write(editor->toPlainText().toUtf8());
        file.close();

        QMessageBox::information(this, "Saved", "XML saved successfully!");

        if (xmlReadyCallback)
            xmlReadyCallback(path);

        close();
    }

    // ================= USE DIRECTLY =================
    void useForOperations()
    {
        QString tempPath = QDir::temp().filePath("temp_written.xml");

        QFile file(tempPath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(editor->toPlainText().toUtf8());
        file.close();

        if (xmlReadyCallback)
            xmlReadyCallback(tempPath);

        close();
    }
};

// ==================== ShowGraph =====================
class Graph : public QWidget {
    QString filePath;

public:
    Graph(QString selectedPath, QWidget* parent = nullptr)
        : QWidget(parent), filePath(selectedPath)
    {
        setWindowTitle("Graph Operations");
        resize(520, 420);

        this->setStyleSheet("background:#020617;");

        // ================= MAIN LAYOUT =================
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(40, 40, 40, 40);
        mainLayout->setSpacing(25);

        // ================= TITLE =================
        QLabel* title = new QLabel("GRAPH FUNCTIONS", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(
            "font-size: 22px;"
            "font-weight: 800;"
            "letter-spacing: 2px;"
            "color: #e5e7eb;"
        );
        mainLayout->addWidget(title);

        // ================= GRID =================
        QGridLayout* grid = new QGridLayout();
        grid->setHorizontalSpacing(25);
        grid->setVerticalSpacing(20);

        fun1 = createButton("Function 1", "#3b82f6");
        fun2 = createButton("Function 2", "#22c55e");
        fun3 = createButton("Function 3", "#f59e0b");
        fun4 = createButton("Function 4", "#ef4444");

        grid->addWidget(fun1, 0, 0);
        grid->addWidget(fun2, 0, 1);
        grid->addWidget(fun3, 1, 0);
        grid->addWidget(fun4, 1, 1);

        mainLayout->addStretch();
        mainLayout->addLayout(grid);
        mainLayout->addStretch();

        applyShadow(fun1);
        applyShadow(fun2);
        applyShadow(fun3);
        applyShadow(fun4);
    }

private:
    QPushButton* fun1;
    QPushButton* fun2;
    QPushButton* fun3;
    QPushButton* fun4;

    // ================= BUTTON STYLE =================
    QPushButton* createButton(const QString& text, const QString& color)
    {
        QPushButton* btn = new QPushButton(text);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedHeight(52);

        btn->setStyleSheet(
            "QPushButton {"
            " background:" + color + ";"
            " color:white;"
            " border-radius:14px;"
            " padding:14px;"
            " font-size:15px;"
            " font-weight:600;"
            "}"
            "QPushButton:hover { background:" + color + "cc; }"
        );

        return btn;
    }

    // ================= SHADOW =================
    void applyShadow(QWidget* w)
    {
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(25);
        shadow->setColor(QColor(0, 0, 0, 120));
        shadow->setOffset(0, 6);
        w->setGraphicsEffect(shadow);
    }
};


// ==================== SHOW FILE WINDOW =====================
class ShowFileWindow : public QWidget {
public:
    ShowFileWindow(QString content, QWidget* parent = nullptr, bool isRawFile = true)
        : QWidget(parent)
    {
        setWindowTitle("File Viewer");
        resize(650, 480);

        this->setStyleSheet("background:#020617;");

        // ================= MAIN LAYOUT =================
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(25, 25, 25, 25);
        layout->setSpacing(18);

        // ================= TITLE =================
        QLabel* title = new QLabel("FILE PREVIEW", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(
            "font-size: 20px;"
            "font-weight: 800;"
            "letter-spacing: 2px;"
            "color: #e5e7eb;"
        );
        layout->addWidget(title);

        // ================= TEXT AREA =================
        QTextEdit* textArea = new QTextEdit(this);
        textArea->setReadOnly(true);
        textArea->setStyleSheet(
            "QTextEdit {"
            " background:#020617;"
            " color:#e5e7eb;"
            " border:1px solid #334155;"
            " border-radius:14px;"
            " font-family: Consolas;"
            " font-size:14px;"
            " padding:14px;"
            "}"
        );

        // ================= LOAD CONTENT =================
        if (isRawFile) {
            QFile file(content);
            if (file.open(QFile::ReadOnly | QFile::Text)) {
                textArea->setText(file.readAll());
            }
            else {
                textArea->setText("❌ Unable to open file.");
            }
        }
        else {
            textArea->setText(content);
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
        resize(950, 650);

        // ================= PAGE =================
        QVBoxLayout* page = new QVBoxLayout(this);
        page->setContentsMargins(25, 25, 25, 25);
        page->setSpacing(20);

        // Dark background
        this->setStyleSheet("background:#020617;");

        // ================= HEADER =================
        QLabel* header = new QLabel("XML OPERATIONS", this);
        header->setAlignment(Qt::AlignCenter);
        header->setStyleSheet(
            "font-size: 26px;"
            "font-weight: 800;"
            "letter-spacing: 2px;"
            "color: #e5e7eb;"
        );
        page->addWidget(header);

        // ================= CARD (BUTTONS) =================
        QFrame* card = new QFrame(this);
        card->setStyleSheet(
            "QFrame {"
            " background: #0f172a;"
            " border-radius: 18px;"
            " padding: 24px;"
            "}"
        );

        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(18);
        page->addWidget(card);

        consistency = createButton("Check Consistency", style()->standardIcon(QStyle::SP_DialogApplyButton), "#ef4444");
        Converting_to_JSON = createButton("Convert to JSON", style()->standardIcon(QStyle::SP_FileIcon), "#14b8a6");
        Formatting = createButton("Format XML", style()->standardIcon(QStyle::SP_BrowserReload), "#3b82f6");
        Compressing = createButton("Compress File", style()->standardIcon(QStyle::SP_ArrowDown), "#facc15");
        Decompressing = createButton("Decompress File", style()->standardIcon(QStyle::SP_ArrowUp), "#9333ea");
        Minifying = createButton("Minify XML", style()->standardIcon(QStyle::SP_ComputerIcon), "#6366f1");
        MisMatchTag =createButton("MisMatchTag",style()->standardIcon(QStyle::SP_MessageBoxWarning), "#ff9800");
        CorrectMismatchTag=createButton("CorrectMismatchTag",style()->standardIcon(QStyle::SP_MediaPlay), "#4caf50");


        QGridLayout* grid = new QGridLayout();
        grid->setHorizontalSpacing(20);
        grid->setVerticalSpacing(15);

        grid->addWidget(consistency, 0, 0);
        grid->addWidget(Converting_to_JSON, 0, 1);
        grid->addWidget(Formatting, 1, 0);
        grid->addWidget(Compressing, 1, 1);
        grid->addWidget(Decompressing, 2, 0);
        grid->addWidget(Minifying, 2, 1);
        grid->addWidget(MisMatchTag, 3, 0);
        grid->addWidget(CorrectMismatchTag, 3, 1);

        cardLayout->addLayout(grid);


        // ================= BEFORE / AFTER =================
        QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
        splitter->setStyleSheet(
            "QSplitter::handle {"
            " background: #111827;"
            " width: 4px;"
            "}"
        );

        beforeEdit = new QTextEdit(this);
        afterEdit  = new QTextEdit(this);

        beforeEdit->setReadOnly(true);
        afterEdit->setReadOnly(true);

        beforeEdit->setStyleSheet(
            "QTextEdit {"
            " background:#1e1e1e;"
            " color:#d4d4d4;"
            " font-family: Consolas;"
            " font-size: 14px;"
            " border-radius: 12px;"
            " padding: 12px;"
            "}"
        );

        afterEdit->setStyleSheet(
            "QTextEdit {"
            " background:#020617;"
            " color:#e5e7eb;"
            " font-family: Consolas;"
            " font-size: 14px;"
            " border-radius: 12px;"
            " padding: 12px;"
            "}"
        );

        // LEFT
        QVBoxLayout* left = new QVBoxLayout();
        QLabel* beforeLabel = new QLabel(" BEFORE XML ");
        beforeLabel->setStyleSheet(
            "QLabel {"
            " background:#374151;"
            " color:#f9fafb;"
            " padding:6px 12px;"
            " border-radius:10px;"
            " font-weight:bold;"
            "}"
        );
        left->addWidget(beforeLabel);
        left->addWidget(beforeEdit);

        QWidget* leftWidget = new QWidget();
        leftWidget->setLayout(left);

        // RIGHT
        QVBoxLayout* right = new QVBoxLayout();
        QLabel* afterLabel = new QLabel(" AFTER RESULT ");
        afterLabel->setStyleSheet(
            "QLabel {"
            " background:#2563eb;"
            " color:white;"
            " padding:6px 12px;"
            " border-radius:10px;"
            " font-weight:bold;"
            "}"
        );
        right->addWidget(afterLabel);
        right->addWidget(afterEdit);

        QWidget* rightWidget = new QWidget();
        rightWidget->setLayout(right);

        splitter->addWidget(leftWidget);
        splitter->addWidget(rightWidget);
        splitter->setStretchFactor(0, 1);
        splitter->setStretchFactor(1, 1);

        //================================================================================

        page->addWidget(splitter);

        // ================= SIGNALS =================
        connect(consistency, &QPushButton::clicked, this, &OperationsWindow::checkConsistency);
        connect(Minifying, &QPushButton::clicked, this, &OperationsWindow::runMinifying);
        connect(Compressing, &QPushButton::clicked, this, &OperationsWindow::runCompressing);
        connect(Decompressing, &QPushButton::clicked, this, &OperationsWindow::runDecompressing);
        connect(Formatting, &QPushButton::clicked, this, &OperationsWindow::runFormatting);
        connect(Converting_to_JSON, &QPushButton::clicked, this, &OperationsWindow::onConvertClicked);
        connect(MisMatchTag,QPushButton::clicked,this,&OperationsWindow::runMismatchTag);
        connect(CorrectMismatchTag,QPushButton::clicked,this,&OperationsWindow::runCorrectMismatchTag);
    }
    void setFilePath(const QString& path) {
        filePath = path;
    }

private:
    QString filePath;
    QPushButton* consistency;
    QPushButton* Converting_to_JSON;
    QPushButton* Formatting;
    QPushButton* Compressing;
    QPushButton* Decompressing;
    QPushButton* Minifying;
    QPushButton* MisMatchTag;////////////////////////////////////////
    QPushButton* CorrectMismatchTag;
    QTextEdit* beforeEdit;
    QTextEdit* afterEdit;

    QPushButton* createButton(QString text, QIcon icon, QString colorHex)
    {
        QPushButton* btn = new QPushButton(icon, text);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setIconSize(QSize(22, 22));
        btn->setStyleSheet(
            "QPushButton {"
            " background:" + colorHex + ";"
            " color:white;"
            " padding:12px;"
            " border-radius:10px;"
            " font-size:14px;"
            " font-weight:bold;"
            " text-align:left;"
            "}"
            "QPushButton:hover { background:" + colorHex + "cc; }"
        );
        return btn;
    }


private slots:
    void updatePreview(const QString& before, const QString& after)
    {
        beforeEdit->setText(before);
        afterEdit->setText(after);
        afterEdit->moveCursor(QTextCursor::Start);
    }

    void checkConsistency()
    {
        string xml = readXMLFile_withSpaces(filePath.toStdString());
        bool ok = checkXMLConsistency(xml);

        QMessageBox::information(
            this,
            "XML Check",
            ok ? "XML is CONSISTENT ✔" : "XML has ERRORS ❌"
        );
    }

    void runMinifying()
    {
        string xml = readXMLFile_withSpaces(filePath.toStdString());
        XMLMinifier minifier;
        updatePreview(QString::fromStdString(xml),
                      QString::fromStdString(minifier.minifyXML(xml)));
    }

    void runFormatting()
    {
        string xml = readXMLFile_withSpaces(filePath.toStdString());
        XMLFormatter formatter;
        updatePreview(QString::fromStdString(xml),
                      QString::fromStdString(formatter.formatXML(xml)));
    }

    void onConvertClicked()
    {
        string xml = readXMLFile_withSpaces(filePath.toStdString());
        SimpleXmlToJson converter;
        updatePreview(QString::fromStdString(xml),
                      QString::fromStdString(converter.convert(xml)));
    }

    void runCompressing()
    {
        QString output = QFileDialog::getSaveFileName(this, "Save", "", "*.bin");
        if (!output.isEmpty())
        {
            compressor c;
            c.compress_file(filePath.toStdString(), output.toStdString());
            QMessageBox::information(this, "Done", "File Compressed Successfully!");
        }
    }

    void runDecompressing()
    {
        QString in = QFileDialog::getOpenFileName(this, "Open", "", "*.bin");
        QString out = QFileDialog::getSaveFileName(this, "Save", "", "*.xml");

        if (!in.isEmpty() && !out.isEmpty())
        {
            decompressor d;
            d.decompress_file(in.toStdString(), out.toStdString());
            QMessageBox::information(this, "Done", "File Decompressed Successfully!");
        }
    }

    void runMismatchTag()
    {
        string xml = readXMLFile_withSpaces(filePath.toStdString());
        vector<pair<size_t, string>> mismatches = findMismatchedTags(xml);


        beforeEdit->setText(QString::fromStdString(xml));

        QString result;

        if (mismatches.empty())
        {
            result = "✔ No mismatched tags found.\nXML is well-formed.";
        }
        else
        {
            result = "❌ Mismatched Tags Found:\n\n";

            for (auto& m : mismatches)
            {
                result +=
                    "Position: " + QString::number(m.first) +
                    " -> " + QString::fromStdString(m.second) + "\n";
            }
        }

        afterEdit->setText(result);
        afterEdit->moveCursor(QTextCursor::Start);
    }

    void runCorrectMismatchTag()
    {
        string xml = readXMLFile_withSpaces(filePath.toStdString());

        // Find mismatches first
        vector<pair<size_t, string>> mismatches = findMismatchedTags(xml);

        beforeEdit->setText(QString::fromStdString(xml));

        // If no mismatches → do nothing
        if (mismatches.empty())
        {
            afterEdit->setText("✔ No mismatched tags found.\nNo correction needed.");
            afterEdit->moveCursor(QTextCursor::Start);
            return;
        }

        // Correct only if mismatches exist
        string correctedXML = correctMismatchedTags(xml);

        QString result;
        result += "✅ Mismatched tags corrected successfully.\n\n";
        result += "---- Corrected XML ----\n\n";
        result += QString::fromStdString(correctedXML);

        afterEdit->setText(result);
        afterEdit->moveCursor(QTextCursor::Start);
    }
};
// ==================== MAIN WINDOW =====================
class Window : public QWidget {
public:
    Window(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowTitle("XML Project");
        resize(520, 420);

        // ================= PAGE =================
        QVBoxLayout* page = new QVBoxLayout(this);
        page->setContentsMargins(30, 30, 30, 30);
        page->setSpacing(22);

        this->setStyleSheet("background:#020617;");

        // ================= HEADER =================
        QLabel* title = new QLabel("XML PROJECT", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(
            "font-size: 28px;"
            "font-weight: 800;"
            "letter-spacing: 2px;"
            "color: #e5e7eb;"
        );
        page->addWidget(title);

        // ================= CARD =================
        QFrame* card = new QFrame(this);
        card->setStyleSheet(
            "QFrame {"
            " background: #0f172a;"
            " border-radius: 18px;"
            " padding: 26px;"
            "}"
        );

        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(22);

        // ================= PRIMARY BUTTONS =================
        QPushButton* selectFile = new QPushButton("Select XML File");
        QPushButton* writeXml   = new QPushButton("Write XML");

        stylePrimaryButton(selectFile, "#3b82f6");
        stylePrimaryButton(writeXml,   "#f97316");

        cardLayout->addWidget(selectFile);
        cardLayout->addWidget(writeXml);

        // ================= ACTION BUTTONS =================
        QHBoxLayout* actions = new QHBoxLayout();
        actions->setSpacing(15);

        QPushButton* operationsBtn = new QPushButton("Operations");
        QPushButton* graphBtn      = new QPushButton("Show Graph");
        QPushButton* showFileBtn   = new QPushButton("Show File");

        styleSecondaryButton(operationsBtn, "#6366f1");
        styleSecondaryButton(graphBtn,      "#f59e0b");
        styleSecondaryButton(showFileBtn,   "#14b8a6");

        actions->addWidget(operationsBtn);
        actions->addWidget(graphBtn);
        actions->addWidget(showFileBtn);

        cardLayout->addLayout(actions);
        page->addWidget(card);

        // ================= SIGNALS =================
        connect(selectFile, &QPushButton::clicked,
                this, &Window::openFileDialog);

        connect(operationsBtn, &QPushButton::clicked,
                this, &Window::processXML);

        connect(showFileBtn, &QPushButton::clicked,
                this, &Window::showSelectedFile);

        connect(graphBtn, &QPushButton::clicked,
                this, &Window::openGraph);

        connect(writeXml, &QPushButton::clicked,
                this, &Window::openWriteXml);
    }

private:
    QString selectedFilePath;
    OperationsWindow* opWindow = nullptr;
    Graph* graph = nullptr;
    WriteXmlWindow* writeWindow = nullptr;

    // ================= STYLES =================
    void stylePrimaryButton(QPushButton* btn, const QString& color) {
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(
            "QPushButton {"
            " background:" + color + ";"
            " color:white;"
            " border-radius:12px;"
            " padding:14px;"
            " font-size:16px;"
            " font-weight:600;"
            "}"
            "QPushButton:hover { background:" + color + "cc; }"
        );
    }

    void styleSecondaryButton(QPushButton* btn, const QString& color) {
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(
            "QPushButton {"
            " background:" + color + ";"
            " color:white;"
            " border-radius:10px;"
            " padding:12px;"
            " font-size:14px;"
            " font-weight:600;"
            "}"
            "QPushButton:hover { background:" + color + "cc; }"
        );
    }

    // ================= LOGIC =================
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
        else
            opWindow->setFilePath(selectedFilePath); // 🔥 تحديث المسار

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

    void openWriteXml() {
        writeWindow = new WriteXmlWindow(
            [this](QString path) {
                selectedFilePath = path;
                QMessageBox::information(
                    this,
                    "XML Ready",
                    "XML is ready for operations."
                );
            }
        );
        writeWindow->show();
    }
};
// ==================== MAIN() =====================
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}

