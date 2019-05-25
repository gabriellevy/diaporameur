#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aspectratiolabel.h"
#include <QMainWindow>
#include <QTimer>
#include <QMovie>
#include <QVector>
#include "diapoimgref.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static QString DOSSIER;

private:
    Ui::MainWindow *ui;

    QVector<DiapoImgRef*> m_HistoriqueDiapoImgRefs;
    QVector<DiapoImgRef*> m_DiapoImgRefs;
    DiapoImgRef* m_DiapoImgActuelle = nullptr;
    PhaseImage m_PhaseImageActuelle = PhaseImage::Intro;
    QTimer* m_Timer;
    int m_BaseDureeMs;

    void CHargerImages();
    /**
     * @brief DeterminerImage
     * @param typeImage
     * @return le chemin d'une image d'un type déterminé
     */
    QString DeterminerImage(TypeImage typeImage);

    void resizeEvent(QResizeEvent* event) override;

    QSize s_TailleImageMax;
    QSize m_TailleInterfaceDeBase;
    void RecalculerTailleImageMax();

    // controles :
    void keyPressEvent(QKeyEvent *event) override;
    bool m_Pause = false;

private slots:
    void RafraichirAffichage(bool chercherNouvelleImage = true);
    void DeclencherDiapo();
};

#endif // MAINWINDOW_H
