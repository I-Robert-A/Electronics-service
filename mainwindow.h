// ======================= mainwindow.h =======================
#pragma once

#include <QMainWindow>

class QLabel;
class QPlainTextEdit;
class QPushButton;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
bool liveEnabled = false;
    struct Impl;
    Impl* impl = nullptr;

    enum class Mode {
        None,
        AngajareTehnician,
        AngajareReceptioner,
        AngajareSupervizor,
        Concediere,
        ModificaNume,
        AdaugaMarca,
        StergeMarca,
        AdaugaModel,
        StergeModel
    };

    Mode mode = Mode::None;

    QLabel* lblInfo = nullptr;
    QPlainTextEdit* editInput = nullptr;   // INPUT multi-line
    QPushButton* btnConfirm = nullptr;
    QPlainTextEdit* outBox = nullptr;
    QTimer* liveTimer = nullptr;

    void setMode(Mode m, const QString& info, const QString& placeholder);
    void showOut(const QString& title, const QString& text);
};
