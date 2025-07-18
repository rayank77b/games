#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <vector>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void generateTips();
    void exportToCSV();

private:
    std::vector<int> generateNumbers(int count, int min, int max);
    QString formatTip(int index, std::vector<int>& mainOut, std::vector<int>& euroOut);

    QVBoxLayout* tipsLayout;
    std::vector<QLabel*> tipLabels;
    std::vector<std::pair<std::vector<int>, std::vector<int>>> tips; // store last result
};