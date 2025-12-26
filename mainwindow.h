#pragma once

#include <QMainWindow>   // <- obligatoriu
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

// dacă EmployeeData / angajat sunt definite în aceste fișiere:
#include "lucruAngajati.h"   // unde e EmployeeData + angajat
// sau include exact headerul care le definește

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // UI
    QLabel* lblInfo = nullptr;
    QLineEdit* editInput = nullptr;
    QPushButton* btnConfirm = nullptr;
    QString tipAngajat;

    // datele tale (persistente în fereastră)
    EmployeeData d;
    std::vector<std::unique_ptr<angajat>> angajati;
    std::unordered_map<std::string,
        std::unordered_map<std::string, std::vector<std::string>>> posReparatii;
};
