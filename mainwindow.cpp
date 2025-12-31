#include "mainwindow.h"

#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTimer>
#include <QStringList>

#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <stdexcept>

// include-urile tale
#include "service.h"
#include "cerereR.h"

// raport global (dacă există)
extern int raport;

// funcțiile tale globale (nu membre service!)
void citireAngajat(std::istream& dev, service& s);
void citire(cerereR& cr, std::string linie, std::ostream& dev, bool optiune);

void afiseazaInvalide(std::istream& dev, std::ostream& devO);
void prelucrareCereri(PQ& cereri, std::ostream& fout2, std::vector<tehnician*>& teh, std::ostream& fout3);
void lunga(std::istream& dev, const std::vector<tehnician*>& tehnicieni);
void salariu(const std::vector<std::unique_ptr<angajat>>& angajati);

// =======================================================
// PIMPL
// =======================================================
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
        finCereri.open("cereri.csv");
        finAngajati.open("angajati.csv");
        finMarci.open("marci.csv");

        foutInvalide.open("cereriInvalide.csv");
        foutIreparabile.open("cereriIreparabile.csv");
        foutReparate.open("cereriReparate");

        if (!finCereri.is_open())   { err="Nu pot deschide cereri.csv"; return false; }
        if (!finAngajati.is_open()) { err="Nu pot deschide angajati.csv"; return false; }
        if (!finMarci.is_open())    { err="Nu pot deschide marci.csv"; return false; }

        if (!foutInvalide.is_open())   { err="Nu pot crea cereriInvalide.csv"; return false; }
        if (!foutIreparabile.is_open()){ err="Nu pot crea cereriIreparabile.csv"; return false; }
        if (!foutReparate.is_open())   { err="Nu pot crea cereriReparate"; return false; }

        // init service
        citireAngajat(finAngajati, s);
        s.citireMarci(finMarci);

        teh = s.getPtrteh();
        rec = s.getPtrrec();
        sup = s.getPtrsup();

        if (teh.size() < 3 || rec.size() < 1 || sup.size() < 1) {
            err = "Service-ul nu are destui angajați (min: 3 teh, 1 rec, 1 sup).";
            return false;
        }

        std::sort(teh.begin(), teh.end(), [](tehnician* a, tehnician* b){
            return a->getLucrare() < b->getLucrare();
        });

        // citește cereri și le pune în PQ
        std::string linie;
        while (std::getline(finCereri, linie)) {
            if (linie.empty()) continue;

            cerereR cr;
            citire(cr, linie, foutInvalide, true);

            // distribuie ID la recepționeri
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

// =======================================================
// UI helpers
// =======================================================
void MainWindow::setMode(Mode m, const QString& info, const QString& placeholder) {
    mode = m;
    lblInfo->setText(info);
    editInput->setPlaceholderText(placeholder);
    editInput->clear();
    editInput->show();
    btnConfirm->show();
    editInput->setFocus();
}
void MainWindow::showOut(const QString& title, const QString& text) {
    outBox->appendPlainText("=== " + title + " ===");
    outBox->appendPlainText(text);
    outBox->appendPlainText("");
}

// =======================================================
// UI parsing helpers
// =======================================================
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

static bool parseIdCsvUI(const QString& s, std::vector<int>& out, QString& err){
    out.clear();
    const QString trimmed = s.trimmed();
    if (trimmed.isEmpty()) { err="Lista ID e goală."; return false; }

    const QStringList ids = trimmed.split(",", Qt::SkipEmptyParts);
    if (ids.isEmpty()) { err="Lista ID invalidă. Ex: 1,2,10"; return false; }

    for (const QString& tok : ids) {
        bool ok=false;
        int v = tok.trimmed().toInt(&ok);
        if (!ok) { err="ID invalid în listă."; return false; }
        out.push_back(v);
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

// =======================================================
// ctor / dtor
// =======================================================
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
    editInput = new QLineEdit(this);
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

    // Menus
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

    QAction* aTop3  = mRapoarte->addAction("Raport: top 3 salarii (terminal)");
    QAction* aLunga = mRapoarte->addAction("Raport: cea mai lungă lucrare (terminal)");
    QAction* aAst   = mRapoarte->addAction("Raport: cereri în așteptare (raport=1)");

    // ---- actions ----
    connect(aTeh, &QAction::triggered, this, [this]{
        setMode(Mode::AngajareTehnician,
                "Angajare Tehnician",
                "Nume;Prenume;CNP;Data(dd/mm/yyyy);Oras;tip;marca|tip;marca|...\n"
                "Ex: Popescu;Ana;2901223123461;17/07/2020;Constanta;TV;LG|frigider;Beko");
    });
    connect(aRec, &QAction::triggered, this, [this]{
        setMode(Mode::AngajareReceptioner,
                "Angajare Recepționer",
                "Nume;Prenume;CNP;Data(dd/mm/yyyy);Oras;ID1,ID2,...\n"
                "Ex: Bratu;Alin;1860922025806;24/05/2022;Bucuresti;1,2,10");
    });
    connect(aSup, &QAction::triggered, this, [this]{
        setMode(Mode::AngajareSupervizor,
                "Angajare Supervizor",
                "Nume;Prenume;CNP;Data(dd/mm/yyyy);Oras\n"
                "Ex: Bratu;Alin;1860922025806;24/05/2022;Bucuresti");
    });

    connect(aConcediaza, &QAction::triggered, this, [this]{
        setMode(Mode::Concediere, "Concediere", "ID\nEx: 7");
    });

    connect(aModNume, &QAction::triggered, this, [this]{
        setMode(Mode::ModificaNume, "Modifică nume", "ID;NumeNou\nEx: 7;Popescu");
    });

    connect(aAddMarca, &QAction::triggered, this, [this]{
        setMode(Mode::AdaugaMarca, "Adaugă marcă", "tip;marca\nEx: TV;Sony");
    });
    connect(aDelMarca, &QAction::triggered, this, [this]{
        setMode(Mode::StergeMarca, "Șterge marcă", "tip;marca\nEx: frigider;Beko");
    });
    connect(aAddModel, &QAction::triggered, this, [this]{
        setMode(Mode::AdaugaModel, "Adaugă model", "tip;marca;model\nEx: TV;Sony;BraviaX90");
    });
    connect(aDelModel, &QAction::triggered, this, [this]{
        setMode(Mode::StergeModel, "Șterge model", "tip;marca;model\nEx: TV;Sony;BraviaX90");
    });

    // Afișare invalide: funcție globală
    connect(aInvalide, &QAction::triggered, this, [this]{
        std::ifstream dev("cereri.csv");
        if (!dev.is_open()) { QMessageBox::warning(this, "Eroare", "Nu pot deschide cereri.csv"); return; }
        std::ostringstream oss;
        afiseazaInvalide(dev, oss);
        showOut("Cererile invalide", QString::fromStdString(oss.str()));
    });

    // LIVE: funcție globală
    connect(aLive, &QAction::triggered, this, [this]{
        if (liveTimer->isActive()) { liveTimer->stop(); lblInfo->setText("LIVE: OFF"); }
        else { liveTimer->start(); lblInfo->setText("LIVE: ON"); }
    });

    connect(aTop3, &QAction::triggered, this, [this]{
        // trebuie să ai acces la vectorul de angajați ca parametru.
        // dacă nu ai getter, fă unul în service (recomandat).
        lblInfo->setText("Top 3 salarii: necesită getter la vectorul de angajați în service.");
    });

    connect(aLunga, &QAction::triggered, this, [this]{
        std::ifstream dev("cereri.csv");
        if (!dev.is_open()) { QMessageBox::warning(this, "Eroare", "Nu pot deschide cereri.csv"); return; }
        lunga(dev, impl->teh);
        lblInfo->setText("Raport lucrare lungă: executat (vezi terminal).");
    });

    connect(aAst, &QAction::triggered, this, [this]{
        raport = 1;
        lblInfo->setText("raport=1 setat.");
        outBox->appendPlainText("raport = 1\n");
    });

    // timer tick: funcție globală
    connect(liveTimer, &QTimer::timeout, this, [this]{
        prelucrareCereri(impl->cereri, impl->foutIreparabile, impl->teh, impl->foutReparate);
    });

    // Confirm
    connect(btnConfirm, &QPushButton::clicked, this, [this]{
        const QString input = editInput->text().trimmed();
        if (input.isEmpty()) { lblInfo->setText("Eroare: input gol."); return; }

        try {
            if (mode == Mode::AngajareTehnician || mode == Mode::AngajareReceptioner || mode == Mode::AngajareSupervizor) {
                QStringList c = input.split(";", Qt::KeepEmptyParts);
                for (auto& x : c) x = x.trimmed();

                int expected = (mode == Mode::AngajareSupervizor) ? 5 : 6;
                if (c.size() != expected) {
                    lblInfo->setText("Eroare: aștept " + QString::number(expected) + " câmpuri separate prin ';'.");
                    return;
                }

                EmployeeData d;
                d.lucrari = 0;
                d.Nume    = c[0].toStdString();
                d.Prenume = c[1].toStdString();
                d.CNP     = c[2].toStdString();
                d.data_A  = c[3].toStdString();
                d.oras_D  = c[4].toStdString();
                d.repara.clear();
                d.IDuri.clear();

                if (mode == Mode::AngajareTehnician) {
                    d.Post = "tehnician";
                    QString perr;
                    if (!parseReparaPairsUI(c[5], d.repara, perr)) { lblInfo->setText("Eroare: " + perr); return; }
                    impl->s.angajareTehnician(d);
                } else if (mode == Mode::AngajareReceptioner) {
                    d.Post = "receptioner";
                    QString perr;
                    if (!parseIdCsvUI(c[5], d.IDuri, perr)) { lblInfo->setText("Eroare: " + perr); return; }
                    impl->s.angajareReceptioner(d);
                } else {
                    d.Post = "supervizor";
                    impl->s.angajareSupervizor(d);
                }

                lblInfo->setText("OK: angajat adăugat.");
                impl->s.afisareAngajati();

                impl->teh = impl->s.getPtrteh();
                impl->rec = impl->s.getPtrrec();
                impl->sup = impl->s.getPtrsup();
            }
            else if (mode == Mode::Concediere) {
                QString perr; int id=0;
                if (!parseIdUI(input, id, perr)) { lblInfo->setText("Eroare: " + perr); return; }
                impl->s.concediere(id);
                lblInfo->setText("OK: concediere.");
                impl->s.afisareAngajati();

                impl->teh = impl->s.getPtrteh();
                impl->rec = impl->s.getPtrrec();
                impl->sup = impl->s.getPtrsup();
            }
            else if (mode == Mode::ModificaNume) {
                // aici îți trebuie un getter în service ca să găsești unique_ptr<angajat>& după ID
                lblInfo->setText("modificaNume: trebuie getter getAngajatByIdRef(ID) în service.");
            }
            else if (mode == Mode::AdaugaMarca || mode == Mode::StergeMarca) {
                std::string tip, marca; QString perr;
                if (!parseTipMarcaUI(input, tip, marca, perr)) { lblInfo->setText("Eroare: " + perr); return; }

                // necesită getter getPosReparatii() în service (altfel nu compilează)
                auto& pr = impl->s.getPosReparatii();
                if (mode == Mode::AdaugaMarca) impl->s.adaugareMarca(tip, marca, pr);
                else                           impl->s.stergereMarca(tip, marca, pr);

                lblInfo->setText("OK: marcă modificată.");
            }
            else if (mode == Mode::AdaugaModel || mode == Mode::StergeModel) {
                std::string tip, marca, model; QString perr;
                if (!parseTipMarcaModelUI(input, tip, marca, model, perr)) { lblInfo->setText("Eroare: " + perr); return; }

                auto& pr = impl->s.getPosReparatii();
                if (mode == Mode::AdaugaModel) impl->s.adaugareModel(tip, marca, model, pr);
                else                           impl->s.stergereModel(tip, marca, model, pr);

                lblInfo->setText("OK: model modificat.");
            }
            else {
                lblInfo->setText("Alege o acțiune.");
            }
        }
        catch (const std::exception& ex) {
            lblInfo->setText(QString("Eroare: ") + ex.what());
        }
    });
}

MainWindow::~MainWindow() {
    delete impl;
    impl = nullptr;
}
