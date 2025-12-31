#pragma once
#include <QMainWindow>

class QLabel;
class QLineEdit;
class QPushButton;
class QPlainTextEdit;
class QTimer;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent=nullptr);
    ~MainWindow() override;

private:
    QLabel* lblInfo=nullptr;
    QLineEdit* editInput=nullptr;
    QPushButton* btnConfirm=nullptr;
    QPlainTextEdit* outBox=nullptr;
    QTimer* liveTimer=nullptr;

    enum class Mode {
        None,
        AngajareTehnician, AngajareReceptioner, AngajareSupervizor,
        Concediere, ModificaNume,
        AdaugaMarca, StergeMarca, AdaugaModel, StergeModel,
        AfiseazaInvalide,
        LiveToggle,
        RaportTop3, RaportLunga, RaportAsteptare,
        ElectrocasniceReparate
    };

    Mode mode = Mode::None;

    // forward către implementarea internă din cpp
    struct Impl;
    Impl* impl = nullptr;

    // helper UI (definite în cpp)
    void setMode(Mode m, const QString& info, const QString& placeholder);
    void showOut(const QString& title, const QString& text);
};
