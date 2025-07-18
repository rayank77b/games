#include "MainWindow.h"
#include <random>
#include <algorithm>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Buttons
    QPushButton* btnDraw = new QPushButton("🎲 Neu ziehen");
    QPushButton* btnExport = new QPushButton("💾 Export als CSV");
    connect(btnDraw, &QPushButton::clicked, this, &MainWindow::generateTips);
    connect(btnExport, &QPushButton::clicked, this, &MainWindow::exportToCSV);

    mainLayout->addWidget(btnDraw);
    mainLayout->addWidget(btnExport);

    // Tippbereich
    tipsLayout = new QVBoxLayout;
    for (int i = 0; i < 10; ++i) {
        QGroupBox* box = new QGroupBox(QString("Tipp %1").arg(i + 1));
        QLabel* label = new QLabel("Noch nichts gezogen");
        QVBoxLayout* boxLayout = new QVBoxLayout;
        boxLayout->addWidget(label);
        box->setLayout(boxLayout);

        tipsLayout->addWidget(box);
        tipLabels.push_back(label);
    }

    mainLayout->addLayout(tipsLayout);
    setCentralWidget(central);
    setWindowTitle("EuroJackpot Generator");
    resize(450, 700);

    generateTips(); // initial ziehen
}

std::vector<int> MainWindow::generateNumbers(int count, int min, int max) {
    std::vector<int> pool;
    for (int i = min; i <= max; ++i) pool.push_back(i);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(pool.begin(), pool.end(), gen);
    return std::vector<int>(pool.begin(), pool.begin() + count);
}

QString MainWindow::formatTip(int index, std::vector<int>& mainOut, std::vector<int>& euroOut) {
    mainOut = generateNumbers(5, 1, 50);
    euroOut = generateNumbers(2, 1, 12);
    std::sort(mainOut.begin(), mainOut.end());
    std::sort(euroOut.begin(), euroOut.end());

    QString result = "Hauptzahlen: ";
    for (int n : mainOut) result += QString::number(n) + " ";
    result += "\nEurozahlen: ";
    for (int e : euroOut) result += QString::number(e) + " ";
    return result;
}

void MainWindow::generateTips() {
    tips.clear();

    for (int i = 0; i < 10; ++i) {
        std::vector<int> mainNums, euroNums;
        QString tipText = formatTip(i, mainNums, euroNums);

        tips.push_back({mainNums, euroNums});
        tipLabels[i]->setText(tipText);
    }
}

void MainWindow::exportToCSV() {
    QFile file("tips.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Tipp,Hauptzahlen,Eurozahlen\n";

        for (long unsigned int i = 0; i < tips.size(); ++i) {
            const auto& [main, euro] = tips[i];

            out << QString::number(i + 1) << ",";
            for (long unsigned int m = 0; m < main.size(); ++m) {
                out << QString::number(main[m]);
                if (m < main.size() - 1) out << " ";
            }
            out << ",";

            for (long unsigned int e = 0; e < euro.size(); ++e) {
                out << QString::number(euro[e]);
                if (e < euro.size() - 1) out << " ";
            }
            out << "\n";
        }

        file.close();
    }
}