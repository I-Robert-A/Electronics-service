
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTimer>
#include <QStringList>
#include <QCoreApplication>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <stdexcept>

#include "../headers/service.h"
#include "../headers/cerereR.h"


void citireAngajat(std::istream& dev, service& s);
void citire(cerereR& cr, std::string linie, std::ostream& dev, bool optiune);

void afiseazaInvalide(std::istream& dev, std::ostream& devO);
void prelucrareCereri(PQ& cereri, std::ostream& fout2, std::vector<tehnician*>& teh, std::ostream& fout3);
void lunga(std::istream& dev, const std::vector<tehnician*>& tehnicieni);
void salariu(const std::vector<std::unique_ptr<angajat>>& angajati);

void afisareReparate(std::ostream& dev, std::istream& devI);


static void logBtn(const char* name) {
    std::cout << "[UI] Click: " << name << std::endl;
}


static QStringList splitCsvLoose(const QString& s) {
    QStringList out = s.split(",", Qt::KeepEmptyParts);
    for (auto& x : out) x = x.trimmed();
    return out;
}
static bool startsWithRole(const QString& x) {
    QString t = x.trimmed().toLower();
    return (t == "tehnician" || t == "receptioner" || t == "supervizor");
}


struct MainWindow::Impl {
    service s;
    PQ cereri;

    std::vector<tehnician*> teh;
    std::vector<receptioner*> rec;
    std::vector<supervizor*> sup;

    std::ifstream finCereri;
    std::ifstream finAngajati;
    std::ifstream finMarci;

    std::ofstream foutInvalide;
    std::ofstream foutIreparabile;
    std::ofstream foutReparate;

    bool init(QString& err) {
        finCereri.open("../tests/cereri.csv");
        finAngajati.open("../tests/angajati.csv");
        finMarci.open("../tests/marci.csv");

        foutInvalide.open("../outputs/cereriInvalide.csv");
        foutIreparabile.open("../outputs/cereriIreparabile.csv");
        foutReparate.open("../outputs/cereriReparate.csv");

        if (!finCereri.is_open())   { err="Nu pot deschide cereri.csv"; return false; }
        if (!finAngajati.is_open()) { err="Nu pot deschide angajati.csv"; return false; }
        if (!finMarci.is_open())    { err="Nu pot deschide marci.csv"; return false; }

        if (!foutInvalide.is_open())    { err="Nu pot crea cereriInvalide.csv"; return false; }
        if (!foutIreparabile.is_open()) { err="Nu pot crea cereriIreparabile.csv"; return false; }
        if (!foutReparate.is_open())    { err="Nu pot crea cereriReparate.csv"; return false; }

        citireAngajat(finAngajati, s);
        s.citireMarci(finMarci);
        

        teh = s.getPtrteh();
        rec = s.getPtrrec();
        sup = s.getPtrsup();

        if (teh.size() < 3 || rec.size() < 1 || sup.size() < 1) {
            err = "Service ul nu are destui angajati.";
            return false;
        }

        std::sort(teh.begin(), teh.end(), [](tehnician* a, tehnician* b){
            return a->getLucrare() < b->getLucrare();
        });

        std::string linie;
        while (std::getline(finCereri, linie)) {
            if (linie.empty()) continue;

            cerereR cr;
            try
            {
                citire(cr, linie, foutInvalide, true);
            }
            catch(const std::exception& e)
            {
                std::stringstream ss(linie);
                std::string idtemp;
                std::getline(ss,idtemp,',');
                std::cerr << e.what() <<" pe liniaC "<<idtemp<< '\n';
                continue;
            }
            if (!rec.empty()) {
                if (rec.size() > 1) {
                    static std::mt19937 gen(std::random_device{}());
                    std::uniform_int_distribution<> dist(0, (int)rec.size() - 1);
                    receptioner* r = rec[dist(gen)];
                    r->addID(cr.getID());
                } else {
                    rec.front()->addID(cr.getID());
                }
            }
            s.verificareCerere(foutInvalide, cr, cereri);
        }

        return true;
    }
};

void MainWindow::setMode(Mode m, const QString& info, const QString& placeholder) {
    mode = m;
    lblInfo->setText(info);

    editInput->setPlaceholderText(placeholder);
    editInput->setPlainText("");
    editInput->show();
    btnConfirm->show();
    editInput->setFocus();
}

void MainWindow::showOut(const QString& title, const QString& text) {
    outBox->appendPlainText("=== " + title + " ===");
    outBox->appendPlainText(text);
    outBox->appendPlainText("");
}


static bool parseReparaPairsUI(const QString& s,
                              std::map<std::string, std::set<std::string>>& repara,
                              QString& err)
{
    repara.clear();
    const QString trimmed = s.trimmed();
    if (trimmed.isEmpty()) { err="Lista reparații e goală."; return false; }

    const QStringList pairs = trimmed.split("|", Qt::SkipEmptyParts);
    if (pairs.isEmpty()) { err="Lista reparații e invalidă."; return false; }

    for (QString pair : pairs) {
        pair = pair.trimmed();
        const QStringList tm = pair.split(";", Qt::KeepEmptyParts);
        if (tm.size()!=2) { err="Format reparații: tip;marca|tip;marca|..."; return false; }

        QString tip = tm[0].trimmed();
        QString marca = tm[1].trimmed();
        if (tip.isEmpty() || marca.isEmpty()) { err="Tip/marcă goală în reparații."; return false; }

        repara[tip.toStdString()].insert(marca.toStdString());
    }
    return true;
}

static bool parseIdUI(const QString& s, int& id, QString& err){
    bool ok=false;
    id = s.trimmed().toInt(&ok);
    if (!ok) { err="ID invalid."; return false; }
    return true;
}

static bool parseTipMarcaUI(const QString& s, std::string& tip, std::string& marca, QString& err){
    auto parts = s.split(";", Qt::KeepEmptyParts);
    if (parts.size()!=2) { err="Format: tip;marca"; return false; }

    QString t=parts[0].trimmed(), m=parts[1].trimmed();
    if (t.isEmpty()||m.isEmpty()) { err="Tip/marcă goală."; return false; }

    tip=t.toStdString(); marca=m.toStdString();
    return true;
}

static bool parseTipMarcaModelUI(const QString& s, std::string& tip, std::string& marca, std::string& model, QString& err){
    auto parts = s.split(";", Qt::KeepEmptyParts);
    if (parts.size()!=3) { err="Format: tip;marca;model"; return false; }

    QString t=parts[0].trimmed(), m=parts[1].trimmed(), mo=parts[2].trimmed();
    if (t.isEmpty()||m.isEmpty()||mo.isEmpty()) { err="Tip/marcă/model gol."; return false; }

    tip=t.toStdString(); marca=m.toStdString(); model=mo.toStdString();
    return true;
}
static bool parseIdNameUI(const QString& s, int& id, QString& numeNou, QString& err)
{
    QString in = s.trimmed();
    if (in.isEmpty()) { err="Input gol."; return false; }

    QStringList parts = in.split(";", Qt::KeepEmptyParts);
    if (parts.size() != 2) parts = in.split(",", Qt::KeepEmptyParts);
    if (parts.size() != 2) { err="Format: ID;NumeNou"; return false; }

    bool ok=false;
    id = parts[0].trimmed().toInt(&ok);
    if (!ok) { err="ID invalid."; return false; }

    numeNou = parts[1].trimmed();
    if (numeNou.isEmpty()) { err="Nume gol."; return false; }

    return true;
}



MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    impl = new Impl();

    auto* cw = new QWidget(this);
    setCentralWidget(cw);

    auto* root = new QVBoxLayout(cw);
    root->setContentsMargins(16,16,16,16);
    root->setSpacing(10);

    lblInfo = new QLabel("Inițializare...", this);

    editInput = new QPlainTextEdit(this);
    editInput->setMaximumHeight(90);
    editInput->hide();

    btnConfirm = new QPushButton("Confirmă", this);
    btnConfirm->hide();

    outBox = new QPlainTextEdit(this);
    outBox->setReadOnly(true);

    liveTimer = new QTimer(this);
    liveTimer->setInterval(700);

    root->addWidget(lblInfo);
    root->addWidget(editInput);
    root->addWidget(btnConfirm);
    root->addWidget(outBox, 1);

    QString err;
    if (!impl->init(err)) {
        lblInfo->setText("Eroare init: " + err);
        QMessageBox::critical(this, "Eroare", err);
        return;
    }
    lblInfo->setText("Init OK.");

    auto* mAngajare = menuBar()->addMenu("Angajare");
    auto* mAngajati = menuBar()->addMenu("Angajați");
    auto* mRep      = menuBar()->addMenu("Reparații");
    auto* mCereri   = menuBar()->addMenu("Cereri");
    auto* mRapoarte = menuBar()->addMenu("Rapoarte");

    QAction* aTeh = mAngajare->addAction("Angajează tehnician");
    QAction* aRec = mAngajare->addAction("Angajează recepționer");
    QAction* aSup = mAngajare->addAction("Angajează supervizor");

    QAction* aConcediaza = mAngajati->addAction("Concediază (ID)");
    QAction* aModNume    = mAngajati->addAction("Modifică nume (ID;NumeNou) [necesită getter în service]");

    QAction* aAddMarca = mRep->addAction("Adaugă marcă (tip;marca)");
    QAction* aDelMarca = mRep->addAction("Șterge marcă (tip;marca)");
    QAction* aAddModel = mRep->addAction("Adaugă model (tip;marca;model)");
    QAction* aDelModel = mRep->addAction("Șterge model (tip;marca;model)");

    QAction* aInvalide = mCereri->addAction("Afișează cereri invalide");
    QAction* aLive     = mCereri->addAction("Procesare LIVE ON/OFF");

    QAction* aTop3     = mRapoarte->addAction("Raport: top 3 salarii (terminal)");
    QAction* aLunga    = mRapoarte->addAction("Raport: cea mai lungă lucrare (terminal)");
    QAction* aReparate = mRapoarte->addAction("Afișează electronice reparate"); // NOU


    connect(aTeh, &QAction::triggered, this, [this]{
        logBtn("Angajează tehnician");
        setMode(Mode::AngajareTehnician,
                "Angajare Tehnician",
                "tehnician,Nume,Prenume,CNP,Data(dd/mm/yyyy),Oras,tip;marca|tip;marca|...\n"
                "Ex: tehnician,Georgescu,Mihai,5050603170065,01/02/2022,Brasov,masina de spalat;Bosch|masina de spalat;LG");
    });

    connect(aRec, &QAction::triggered, this, [this]{
        logBtn("Angajează recepționer");
        setMode(Mode::AngajareReceptioner,
                "Angajare Recepționer",
                "receptioner,Nume,Prenume,CNP,Data(dd/mm/yyyy),Oras,ID1;ID2;...\n"
                "Ex: receptioner,Bratu,Alin,1860922025806,24/05/2022,Bucuresti,1;2;10");
    });
    connect(aTop3, &QAction::triggered, this, [this]{
    std::ostringstream oss;

    auto* oldBuf = std::cout.rdbuf(oss.rdbuf());

    salariu(impl->s.getAngajati());

    std::cout.rdbuf(oldBuf); 

    showOut("Top 3 salarii", QString::fromStdString(oss.str()));
    lblInfo->setText("Top 3 salarii afișat.");
});

    connect(aSup, &QAction::triggered, this, [this]{
        logBtn("Angajează supervizor");
        setMode(Mode::AngajareSupervizor,
                "Angajare Supervizor",
                "supervizor,Nume,Prenume,CNP,Data(dd/mm/yyyy),Oras\n"
                "Ex: supervizor,Bratu,Alin,1860922025806,24/05/2022,Bucuresti");
    });

    connect(aConcediaza, &QAction::triggered, this, [this]{
        logBtn("Concediază");
        setMode(Mode::Concediere, "Concediere", "ID\nEx: 7");
    });

    connect(aModNume, &QAction::triggered, this, [this]{
    setMode(Mode::ModificaNume,
            "Modifică nume",
            "ID;NumeNou\nEx: 7;Popescu");
});

    connect(aAddMarca, &QAction::triggered, this, [this]{
        logBtn("Adaugă marcă");
        setMode(Mode::AdaugaMarca, "Adaugă marcă", "tip;marca\nEx: masina de spalat;Bosch");
    });

    connect(aDelMarca, &QAction::triggered, this, [this]{
        logBtn("Șterge marcă");
        setMode(Mode::StergeMarca, "Șterge marcă", "tip;marca\nEx: frigider;Beko");
    });

    connect(aAddModel, &QAction::triggered, this, [this]{
        logBtn("Adaugă model");
        setMode(Mode::AdaugaModel, "Adaugă model", "tip;marca;model\nEx: TV;Sony;BraviaX90");
    });

    connect(aDelModel, &QAction::triggered, this, [this]{
        logBtn("Șterge model");
        setMode(Mode::StergeModel, "Șterge model", "tip;marca;model\nEx: TV;Sony;BraviaX90");
    });

    connect(aInvalide, &QAction::triggered, this, [this]{
        logBtn("Afișează cereri invalide");
        std::ifstream dev("../outputs/cereriInvalide.csv");
        if (!dev.is_open()) { QMessageBox::warning(this, "Eroare", "Nu pot deschide cereri.csv"); return; }

        std::ostringstream oss;
        try {
            afiseazaInvalide(dev, oss);
            showOut("Cererile invalide", QString::fromStdString(oss.str()));
        } catch (const std::exception& ex) {
            QString msg = QString("afiseazaInvalide a aruncat excepție: ") + ex.what();
            lblInfo->setText(msg);
            QMessageBox::warning(this, "Eroare", msg);
            std::cout << "[UI] EXCEPTION afiseazaInvalide: " << ex.what() << std::endl;
        }
    });

    connect(aLive, &QAction::triggered, this, [this]{
    if (liveTimer->isActive()) {
        liveEnabled = false;
        liveTimer->stop();
        lblInfo->setText("LIVE: OFF");
    } else {
        liveEnabled = true;
        liveTimer->start();
        lblInfo->setText("LIVE: ON");
    }
});

    connect(aLunga, &QAction::triggered, this, [this]{
        logBtn("Raport lucrare lungă");
        std::ifstream dev("../outputs/cereriReparate.csv");
        if (!dev.is_open()) { QMessageBox::warning(this, "Eroare", "Nu pot deschide cereriReparate.csv"); return; }
        try
        {
        lunga(dev, impl->teh);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        lblInfo->setText("Raport lucrare lungă: executat (vezi terminal).");
    });

    connect(aReparate, &QAction::triggered, this, [this]{
        logBtn("Afișează electronice reparate");

        std::ifstream devI("../outputs/cereriReparate.csv");
        if (!devI.is_open()) {
            QMessageBox::warning(this, "Eroare", "Nu pot deschide cereriReparate.csv");
            return;
        }

        afisareReparate(std::cout, devI);

        devI.clear();
        devI.seekg(0);

        std::ostringstream oss;
        afisareReparate(oss, devI);
        showOut("Electronice reparate", QString::fromStdString(oss.str()));
    });

    connect(liveTimer, &QTimer::timeout, this, [this]{
    if (!liveEnabled) return;

    if (impl->cereri.empty()) {
        liveEnabled = false;
        liveTimer->stop();
        lblInfo->setText("LIVE: OFF (fără cereri)");
        return;
    }

    prelucrareCereri(impl->cereri, impl->foutIreparabile, impl->teh, impl->foutReparate);

    if (impl->cereri.empty()) {
        liveEnabled = false;
        liveTimer->stop();
        lblInfo->setText("LIVE: OFF (procesare terminată)");
    }
}); 

    connect(btnConfirm, &QPushButton::clicked, this, [this]{
        logBtn("Confirmă");

        const QString input = editInput->toPlainText().trimmed();
        if (input.isEmpty()) { lblInfo->setText("Eroare: input gol."); return; }

        try {
            if (mode == Mode::AngajareTehnician || mode == Mode::AngajareReceptioner || mode == Mode::AngajareSupervizor) {

                QStringList c = splitCsvLoose(input);
                if (c.isEmpty()) { lblInfo->setText("Eroare: input gol."); return; }

                int offset = 0;
                if (startsWithRole(c[0])) offset = 1;

                int expected = (mode == Mode::AngajareSupervizor) ? (offset + 5) : (offset + 6);
                if (c.size() != expected) {
                    lblInfo->setText("Eroare: format invalid. Aștept " + QString::number(expected) +
                                     " câmpuri separate prin ',' (virgulă).");
                    return;
                }

                EmployeeData d;
                d.lucrari = 0;

                d.Nume    = c[offset + 0].toStdString();
                d.Prenume = c[offset + 1].toStdString();
                d.CNP     = c[offset + 2].toStdString();
                d.data_A  = c[offset + 3].toStdString();
                d.oras_D  = c[offset + 4].toStdString();
                d.repara.clear();
                d.IDuri.clear();
                d.pretR=0.0;

                if (mode == Mode::AngajareTehnician) {
                    d.Post = "tehnician";
                    QString perr;
                    if (!parseReparaPairsUI(c[offset + 5], d.repara, perr)) {
                        lblInfo->setText("Eroare reparații: " + perr);
                        return;
                    }
                    impl->s.angajareTehnician(d);
                }
                else if (mode == Mode::AngajareReceptioner) {
                    d.Post = "receptioner";

                    QString idsField = c[offset + 5].trimmed();
                    if (idsField.contains(',')) {
                        lblInfo->setText("Eroare: ID-urile la recepționer trebuie într-un singur câmp, ex: 1;2;10");
                        return;
                    }

                    idsField = idsField.replace(" ", ";");
                    QStringList parts = idsField.split(";", Qt::SkipEmptyParts);

                    std::vector<int> ids;
                    ids.reserve(parts.size());
                    for (const auto& p : parts) {
                        bool ok=false;
                        int v = p.trimmed().toInt(&ok);
                        if (!ok) { lblInfo->setText("Eroare: ID invalid în listă."); return; }
                        ids.push_back(v);
                    }
                    d.IDuri = ids;

                    impl->s.angajareReceptioner(d);
                }
                else {
                    d.Post = "supervizor";
                    impl->s.angajareSupervizor(d);
                }

                lblInfo->setText("OK: angajat adăugat.");

                std::ostringstream oss;
                impl->s.afisareAngajati(oss);
                showOut("Lista angajați", QString::fromStdString(oss.str()));
                impl->s.afisareAngajati(std::cout);

                impl->teh = impl->s.getPtrteh();
                impl->rec = impl->s.getPtrrec();
                impl->sup = impl->s.getPtrsup();
            }

            else if (mode == Mode::Concediere) {
                QString perr; int id=0;
                if (!parseIdUI(input, id, perr)) { lblInfo->setText("Eroare: " + perr); return; }

                if(impl->s.concediere(id))
                {   lblInfo->setText("OK: concediere.");
                    std::ostringstream oss;
                    impl->s.afisareAngajati(oss);
                    showOut("Lista angajați", QString::fromStdString(oss.str()));
                    impl->s.afisareAngajati(std::cout);

                    impl->teh = impl->s.getPtrteh();
                    impl->rec = impl->s.getPtrrec();
                    impl->sup = impl->s.getPtrsup();
                    if(impl->teh.size() < 3 || impl->rec.size() < 1 || impl->sup.size() < 1)
                    {
                        std::cout << "Service ul nu are destui angajati" << std::endl;
                        // Închidem aplicația Qt elegant
                        QCoreApplication::quit(); 
                        return; 
                    }
                }
                else
                {
                    lblInfo->setText("Nu OK: concediere.");
                }

                
            }

            else if (mode == Mode::ModificaNume) {
    QString err, numeNou;
    int id = 0;

    if (!parseIdNameUI(input, id, numeNou, err)) {
        lblInfo->setText("Eroare: " + err);
        return;
    }

    impl->s.modificaNume(id, numeNou.toStdString());
    lblInfo->setText("OK: nume modificat.");

    std::ostringstream oss;
    impl->s.afisareAngajati(oss);
    showOut("Angajați", QString::fromStdString(oss.str()));
}


            else if (mode == Mode::AdaugaMarca || mode == Mode::StergeMarca) {
                std::string tip, marca; QString perr;
                if (!parseTipMarcaUI(input, tip, marca, perr)) { lblInfo->setText("Eroare: " + perr); return; }

                auto& pr = impl->s.getPosReparatii();
                if (mode == Mode::AdaugaMarca) impl->s.adaugareMarca(tip, marca, pr);
                else                           impl->s.stergereMarca(tip, marca, pr);

                lblInfo->setText("OK: marcă modificată.");
                std::cout << "[UI] OK: marcă modificată (" << tip << ";" << marca << ")\n";
            }


            else if (mode == Mode::AdaugaModel || mode == Mode::StergeModel) {
                std::string tip, marca, model; QString perr;
                if (!parseTipMarcaModelUI(input, tip, marca, model, perr)) { lblInfo->setText("Eroare: " + perr); return; }

                auto& pr = impl->s.getPosReparatii(); 
                if (mode == Mode::AdaugaModel) impl->s.adaugareModel(tip, marca, model, pr);
                else                           impl->s.stergereModel(tip, marca, model, pr);

                lblInfo->setText("OK: model modificat.");
                std::cout << "[UI] OK: model modificat (" << tip << ";" << marca << ";" << model << ")\n";
            }

            else {
                lblInfo->setText("Alege o acțiune.");
            }
        }
        catch (const std::exception& ex) {
            lblInfo->setText(QString("Eroare: ") + ex.what());
            std::cout << "[UI] EXCEPTION: " << ex.what() << std::endl;
        }
    });
}

MainWindow::~MainWindow()
{
    try {
        std::ofstream foutA("../tests/angajati.csv");
        if (foutA.is_open()) {
            impl->s.afisareAngajatiCSV(foutA);
            foutA.close();
        }
            std::ofstream foutMM("../tests/marci.csv");
            if (foutMM.is_open())
                impl->s.inregistrareMM(foutMM);
    } catch (...) {
        
    }

    delete impl;
    impl = nullptr;
}

