#include <QLabel>
#include <fstream>

#include <QProcess>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QSplitter>
#include <QStyle>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QInputDialog>
#include "xmlCheck.h"
#include "minifier.h"
#include "compression_decompression.h"
#include "formatter.h"
#include "SimpleXmlToJson.h"
#include "graph.h"
#include <QTextEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QPropertyAnimation>


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
class GraphWindow : public QWidget
{
public:
    explicit GraphWindow(graph<string>* g, QWidget* parent = nullptr)
        : QWidget(parent), socialGraph(g)
    {
        setWindowTitle("Graph Operations");
        resize(1000, 700);
        setStyleSheet("background:#020617;");

        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(30, 30, 30, 30);
        mainLayout->setSpacing(20);

        // ===== TITLE =====
        QLabel* title = new QLabel("GRAPH FUNCTIONS", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(
            "font-size:24px;"
            "font-weight:900;"
            "letter-spacing:3px;"
            "color:#38bdf8;"
        );
        mainLayout->addWidget(title);

        // ===== BUTTONS =====
        QGridLayout* grid = new QGridLayout();
        grid->setSpacing(15);

        btnInfluencer  = createButton("Most Influencer", "#3b82f6");
        btnActive      = createButton("Most Active", "#22c55e");
        btnMutual      = createButton("Mutual Followers", "#f59e0b");
        btnSuggest     = createButton("Suggest Users", "#ef4444");
        btnWordSearch  = createButton("Search Word", "#8b5cf6");
        btnTopicSearch = createButton("Search Topic", "#14b8a6");
        btnClear       = createButton("Clear Output", "#64748b");
        btnexport      = createButton("Export Graph", "#22c55e");

        grid->addWidget(btnInfluencer,  0, 0);
        grid->addWidget(btnActive,      0, 1);
        grid->addWidget(btnWordSearch,  0, 2);
        grid->addWidget(btnClear,       0, 3);

        grid->addWidget(btnMutual,      1, 0);
        grid->addWidget(btnSuggest,     1, 1);
        grid->addWidget(btnTopicSearch, 1, 2);
        grid->addWidget(btnexport,      1, 3);

        mainLayout->addLayout(grid);

        // ===== IMAGE =====
        imageLabel = new QLabel(this);
        imageLabel->setMinimumSize(800, 450);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setStyleSheet(
            "border:2px dashed #334155;"
            "border-radius:16px;"
        );
        mainLayout->addWidget(imageLabel);

        // ===== TABLE =====
        outputTable = new QTableWidget(this);
        outputTable->setColumnCount(3);
        outputTable->setHorizontalHeaderLabels(
            {"User ID", "User Name", "Content"}
        );
        outputTable->horizontalHeader()->setStretchLastSection(true);
        outputTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        outputTable->verticalHeader()->setVisible(false);
        outputTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        outputTable->setStyleSheet(
            "QTableWidget {"
            " background:#020617;"
            " color:#e5e7eb;"
            " gridline-color:#334155;"
            " border-radius:12px;"
            "}"
            "QHeaderView::section {"
            " background:#0f172a;"
            " color:#38bdf8;"
            " font-weight:700;"
            "}"
        );

        mainLayout->addWidget(outputTable);

        // ===== CONNECTIONS =====
        connect(btnInfluencer,  &QPushButton::clicked, this, &GraphWindow::runbtnInfluencer);
        connect(btnActive,      &QPushButton::clicked, this, &GraphWindow::runbtnActive);
        connect(btnMutual,      &QPushButton::clicked, this, &GraphWindow::runbtnMutual);
        connect(btnSuggest,     &QPushButton::clicked, this, &GraphWindow::runbtnSuggest);
        connect(btnWordSearch,  &QPushButton::clicked, this, &GraphWindow::runbtnWordSearch);
        connect(btnTopicSearch, &QPushButton::clicked, this, &GraphWindow::runbtnTopicSearch);
        connect(btnClear,       &QPushButton::clicked, this, &GraphWindow::clearOutput);
        connect(btnexport,      &QPushButton::clicked, this, &GraphWindow::runbtnExport);
    }

private:
    graph<string>* socialGraph;
    QPushButton *btnInfluencer, *btnActive, *btnMutual,
                *btnSuggest, *btnWordSearch,
                *btnTopicSearch, *btnClear, *btnexport;

    QTableWidget* outputTable;
    QLabel* imageLabel;

    QPushButton* createButton(const QString& text, const QString& color)
    {
        QPushButton* btn = new QPushButton(text, this);
        btn->setFixedHeight(46);
        btn->setStyleSheet(
            "QPushButton {"
            " background:" + color + ";"
            " color:white;"
            " border-radius:14px;"
            " font-weight:600;"
            "}"
        );
        return btn;
    }

    // ===== SLOTS =====
    void clearOutput()
    {
        outputTable->setRowCount(0);
    }



    bool convertDotToJpg(const QString& dotFile, const QString& pngFile)
    {
        QProcess p;
        // CHANGE: -Tjpg to -Tpng
        p.start("dot", {"-Tpng", dotFile, "-o", pngFile});
        p.waitForFinished();
        // OPTIONAL: Check for errors from QProcess
        if (p.exitCode() != 0) {
            // You can print the error output for debugging
            // cerr << "Graphviz Error: " << p.readAllStandardError().toStdString() << endl;
            return false;
        }
        return QFile::exists(pngFile);
    }

    void runbtnExport()
    {
        QString dotPath = QFileDialog::getSaveFileName(
            this, "Save Graph", "graph.dot", "DOT Files (*.dot)"
        );
        if (dotPath.isEmpty()) return;

        QString pngPath = dotPath;
        // CHANGE: .jpg to .png
        pngPath.replace(".dot", ".png");

        socialGraph->exportToDot(dotPath.toStdString());

        // CHANGE: dotPath, jpgPath to dotPath, pngPath
        if (!convertDotToJpg(dotPath, pngPath))
        {
            addRow("Export", "Error", "Graphviz failed");
            return;
        }

        // CHANGE: jpgPath to pngPath
        showGraphImage(pngPath);
    }

    void showGraphImage(const QString& path)
    {
        QPixmap pix(path);
        if (pix.isNull())
        {
            addRow("Image Error", "Status", "Failed to load image");
            return;
        }

        imageLabel->setPixmap(
            pix.scaled(
                imageLabel->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            )
        );

        addRow("Export", "Success", "Graph image displayed");
    }

    void addRow(const QString& a, const QString& b, const QString& c)
    {
        int r = outputTable->rowCount();
        outputTable->insertRow(r);
        outputTable->setItem(r, 0, new QTableWidgetItem(a));
        outputTable->setItem(r, 1, new QTableWidgetItem(b));
        outputTable->setItem(r, 2, new QTableWidgetItem(c));
    }


void runbtnInfluencer()
{
    clearOutput();
    auto r = socialGraph->Most_Influencer();

    if (r.first.empty())
    {
        addRow("Info", "-", "No influencer found");
        return;
    }

    addRow(
        QString::fromStdString(r.first),
        QString::fromStdString(r.second),
        "Top Influencer"
    );
}

void runbtnActive()
{
    clearOutput();
    auto r = socialGraph->Most_Active();

    if (r.first.empty())
    {
        addRow("Info", "-", "No active user found");
        return;
    }

    addRow(
        QString::fromStdString(r.first),
        QString::fromStdString(r.second),
        "Most Active User"
    );
}

void runbtnMutual()
{
    clearOutput();

    bool ok;
    QString input = QInputDialog::getText(
        this,
        "Mutual Followers",
        "Enter IDs (1,2,3):",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (!ok || input.trimmed().isEmpty())
        return;

    vector<string> users;
    for (auto& s : input.split(","))
        users.push_back(s.trimmed().toStdString());

    auto res = socialGraph->MutualFollowers(users);

    if (res.empty())
    {
        addRow("Info", "-", "No mutual followers");
        return;
    }

    for (auto& p : res)
    {
        addRow(
            QString::fromStdString(p.first),
            QString::fromStdString(p.second),
            "Mutual Follower"
        );
    }
}

void runbtnSuggest()
{
    clearOutput();

    bool ok;
    QString id = QInputDialog::getText(
        this,
        "Suggest Users",
        "Enter User ID:",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (!ok || id.trimmed().isEmpty())
        return;

    auto res = socialGraph->Suggest(id.toStdString());

    if (res.empty())
    {
        addRow("Info", "-", "No suggestions found");
        return;
    }

    for (auto& p : res)
    {
        addRow(
            QString::fromStdString(p.first),
            QString::fromStdString(p.second),
            "Suggested User"
        );
    }
}

void runbtnWordSearch()
{
    clearOutput();

    bool ok;
    QString word = QInputDialog::getText(
        this,
        "Search Word",
        "Enter word:",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (!ok || word.trimmed().isEmpty())
        return;

    auto res = socialGraph->WordPostSearch(word.toStdString());

    if (res.empty())
    {
        addRow("Info", "-", "No posts found");
        return;
    }

    for (auto& t : res)
    {
        addRow(
            QString::fromStdString(get<0>(t)),
            QString::fromStdString(get<1>(t)),
            QString::fromStdString(get<2>(t))
        );
    }
}

void runbtnTopicSearch()
{
    clearOutput();

    bool ok;
    QString topic = QInputDialog::getText(
        this,
        "Search Topic",
        "Enter topic:",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (!ok || topic.trimmed().isEmpty())
        return;

    auto res = socialGraph->topicPostSearch(topic.toStdString());

    if (res.empty())
    {
        addRow("Info", "-", "No posts found");
        return;
    }

    for (auto& t : res)
    {
        addRow(
            QString::fromStdString(get<0>(t)),
            QString::fromStdString(get<1>(t)),
            QString::fromStdString(get<2>(t))
        );
    }
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
        SaveResult = createButton("Save Result",style()->standardIcon(QStyle::SP_DialogSaveButton),"#22c55aa");



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
        grid->addWidget(SaveResult, 4, 0, 1, 2);

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
        connect(SaveResult, &QPushButton::clicked,this, &OperationsWindow::saveResultToFile);

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
    QPushButton* MisMatchTag;
    QPushButton* CorrectMismatchTag;
    QPushButton* SaveResult;
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
            " text-align:center;"
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

        vector<pair<size_t, string>> mismatches = findMismatchedTags(xml);

        beforeEdit->setText(QString::fromStdString(xml));

        if (mismatches.empty())
        {
            afterEdit->setText("✔ No mismatched tags found.\nNo correction needed.");
            afterEdit->moveCursor(QTextCursor::Start);
            return;
        }

        // Correct XML
        string correctedXML = correctMismatchedTags(xml);

        // Write back to the same file
        bool saved = writeXMLFile(filePath.toStdString(), correctedXML);

        QString result;
        if (saved)
        {
            result += "✅ Mismatched tags corrected successfully.\n";
            result += "💾 File updated successfully.\n\n";
            result += "---- Corrected XML ----\n\n";
            result += QString::fromStdString(correctedXML);
        }
        else
        {
            result += "❌ Error: Could not save corrected XML to file.\n";
        }

        afterEdit->setText(result);
        afterEdit->moveCursor(QTextCursor::Start);
    }

    bool writeXMLFile(const string& path, const string& content)
    {
        ofstream out(path);
        if (!out.is_open())
            return false;

        out << content;
        out.close();
        return true;
    }

    void saveResultToFile()
    {
        QString content = afterEdit->toPlainText().trimmed();

        if (content.isEmpty())
        {
            QMessageBox::warning(this, "Save",
                                 "No result to save.");
            return;
        }

        QString fileName = QFileDialog::getSaveFileName(
            this,
            "Save Result",
            "",
            "XML File (*.xml);;JSON File (*.json)"
        );

        if (fileName.isEmpty())
            return;

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, "Error",
                                  "Could not save file.");
            return;
        }

        QTextStream out(&file);

        // لو المستخدم اختار JSON
        if (fileName.endsWith(".json", Qt::CaseInsensitive))
        {
            // لو afterEdit فيه XML → نحوله JSON
            if (content.trimmed().startsWith("<"))
            {
                SimpleXmlToJson converter;
                out << QString::fromStdString(
                    converter.convert(content.toStdString())
                );
            }
            else
            {
                out << content; // أصلاً JSON
            }
        }
        else // XML
        {
            out << content;
        }

        file.close();

        QMessageBox::information(this, "Saved",
                                 "File saved successfully ✔");
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
        QPushButton* graphBtn      = new QPushButton("convertToGraph");
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
    GraphWindow* graphWindow = nullptr;
    WriteXmlWindow* writeWindow = nullptr;
    graph<string>* socialGraph = nullptr;


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

    void openGraph()
    {
        if (selectedFilePath.isEmpty()) {
            QMessageBox::warning(this, "Error", "Select a file first.");
            return;
        }

        // Convert QString -> std::string
        string xmlPath = selectedFilePath.toStdString();

        // Create graph (adjust size if needed)
        if (socialGraph)
            delete socialGraph;

        socialGraph = new graph<string>(1000); // max vertices

        // Convert XML -> Graph
        socialGraph->convertXMLtoGraph(xmlPath);

        QMessageBox::information(
            this,
            "Graph Created",
            "XML converted to graph successfully!"
        );

        // Optional: show GraphWindow
        if (graphWindow) {
            graphWindow->close();
            delete graphWindow;
        }

        graphWindow = new GraphWindow(socialGraph);
        graphWindow->show();

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
