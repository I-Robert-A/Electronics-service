/*#include <iostream>
#include <exception>
#include "lucruAngajati.h"
#include "service.h"

service* service::instance=nullptr;

int main() {
     EmployeeData d{
            "Popa", "Iona", "5050603170065",
            "10/03/2031", "Cluj",
            {"TV"}, {"LG"}, {1,3}
     };
    std::vector<std::unique_ptr<angajat>> angajati;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> posReparatii;
    angajare(d,"tehnician",angajati);
    for (const auto& p : angajati) {      // iterare prin referință
    if (p) p->afisare(std::cout);     // p e unique_ptr, folosești ->
    }
    concediere(1,angajati);
    for (const auto& p : angajati) {      // iterare prin referință
    if (p) p->afisare(std::cout);     // p e unique_ptr, folosești ->
    }
    std::string tip="frigider";
    std::string tip2="TV";
    std::string marca="LG";
    std::string model="pinguin";
    std::string model2="pasare";
    adaugareModel(tip,marca,model,posReparatii);
    adaugareModel(tip,marca,model2,posReparatii);
    stergereMarca(tip,marca,posReparatii);
    adaugareMarca(tip2,"Samsung",posReparatii);

    for(auto it = posReparatii.begin(); it != posReparatii.end(); it++)
        {
            std::cout << it->first << ": ";
            for(auto it2=(*it).second.begin();it2!=(*it).second.end();it2++)
                {
                    std::cout<<it2->first<<"="<<" ";
                    for(auto it3:(*it2).second)
                    {
                        std::cout<<(it3)<<" ";
                    }
                }
        }
    return 0;
}
*/
// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <iostream>
#include <exception>
#include "lucruAngajati.h"
#include "service.h"

service* service::instance=nullptr;
static std::vector<std::string> splitToStdVector(const QString& s, QChar sep)
{
    QStringList parts = s.split(sep, Qt::SkipEmptyParts);
    for (QString& p : parts) p = p.trimmed();

    std::vector<std::string> out;
    out.reserve(parts.size());
    for (const auto& p : parts) out.push_back(p.toStdString());
    return out;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    // Dacă nu ai centralWidget în .ui, creează unul:
    if (!centralWidget()) {
        setCentralWidget(new QWidget(this));
    }

    auto* root = new QVBoxLayout(centralWidget());
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(10);

    // Buton principal
    auto* btnAdauga = new QPushButton("Adaugă angajat", this);

    // Meniu dropdown
    auto* menu = new QMenu(this);
    auto* actTehnician   = menu->addAction("Tehnician");
    auto* actSupervizor  = menu->addAction("Supervizor");
    auto* actReceptioner = menu->addAction("Recepționer");

    btnAdauga->setMenu(menu);

    // Label informativ
    lblInfo = new QLabel("Alege un tip de angajat.", this);

    // Input pentru date (ascuns până alegi tipul)
    editInput = new QLineEdit(this);
    editInput->setPlaceholderText("Date separate prin ;");
    editInput->hide();

    // Confirmare (ascuns până alegi tipul)
    btnConfirm = new QPushButton("Confirmă", this);
    btnConfirm->hide();

    // Layout
    root->addWidget(btnAdauga);
    root->addWidget(lblInfo);
    root->addWidget(editInput);
    root->addWidget(btnConfirm);
    root->addStretch(1);

    // Funcție locală: când selectezi tipul
    auto onTipSelectat = [this](const QString& tip) {
        tipAngajat = tip;
        lblInfo->setText("Tip ales: " + tipAngajat);

        // Poți schimba aici formatul exact pe care îl aștepți
        if (tipAngajat == "Tehnician") {
            editInput->setPlaceholderText("Tehnician (7): c1;c2;c3;c4;c5;c6;c7");
        } else if (tipAngajat == "Supervizor") {
            editInput->setPlaceholderText("Supervizor (5): c1;c2;c3;c4;c5");
        } else if (tipAngajat == "Recepționer") {
            editInput->setPlaceholderText("Recepționer (6): c1;c2;c3;c4;c5;c6");
        } else {
            editInput->setPlaceholderText("Date separate prin ;");
        }

        editInput->clear();
        editInput->show();
        btnConfirm->show();
        editInput->setFocus();
    };

    connect(actTehnician, &QAction::triggered, this, [=]() { onTipSelectat("Tehnician"); });
    connect(actSupervizor, &QAction::triggered, this, [=]() { onTipSelectat("Supervizor"); });
    connect(actReceptioner, &QAction::triggered, this, [=]() { onTipSelectat("Recepționer"); });

    // Confirmare: parsare + validare + afișare în terminal
connect(btnConfirm, &QPushButton::clicked, this, [this]() {

    QString input = editInput->text().trimmed();
    if (tipAngajat.isEmpty()) { lblInfo->setText("Eroare: nu ai ales tipul."); return; }
    if (input.isEmpty()) { lblInfo->setText("Eroare: câmp gol."); return; }

    QStringList campuri = input.split(";", Qt::KeepEmptyParts);
    for (QString& c : campuri) c = c.trimmed();

    int expected = -1;
    if (tipAngajat == "Tehnician") expected = 7;
    else if (tipAngajat == "Supervizor") expected = 5;
    else if (tipAngajat == "Recepționer") expected = 6;

    if (campuri.size() != expected) {
        lblInfo->setText("Eroare: " + tipAngajat + " trebuie să aibă exact "
                         + QString::number(expected) + " câmpuri.");
        return;
    }

    // câmpuri comune
    d.Nume    = campuri[0].toStdString();
    d.Prenume = campuri[1].toStdString();
    d.CNP     = campuri[2].toStdString();
    d.data_A  = campuri[3].toStdString();
    d.oras_D  = campuri[4].toStdString();

    if (tipAngajat == "Tehnician") {
        d.tipuriR = splitToStdVector(campuri[5], ',');
        d.marciR  = splitToStdVector(campuri[6], ',');
    } else if (tipAngajat == "Recepționer") {
        QStringList ids = campuri[5].split(",", Qt::SkipEmptyParts);
        d.IDuri.clear();
        for (QString id : ids) {
            bool ok = false;
            int val = id.trimmed().toInt(&ok);
            if (!ok) { lblInfo->setText("Eroare: ID-uri invalide."); return; }
            d.IDuri.push_back(val);
        }
    }

    // Angajează în funcție de tipul ales
    angajare(d, tipAngajat.toLower().toStdString(), angajati);

    // Afișează în terminal
    for (const auto& p : angajati) {
        if (p) p->afisare(std::cout);
    }

    lblInfo->setText("Angajat adăugat ✔ (vezi terminalul)");
});

}

MainWindow::~MainWindow()
{
    delete ui;
}
