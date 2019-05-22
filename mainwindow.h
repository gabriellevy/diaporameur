#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aspectratiolabel.h"
#include <QMainWindow>
#include <QTimer>
#include <QMovie>

namespace Ui {
class MainWindow;
}

enum TypeImage {
    ImgFixe,
    ImgAnim,
    Toutes
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString m_Dossier = "E:/pic/";
    QStringList m_ImagesFixes;
    QStringList m_Gifs;
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
    void RafraichirAffichage();
    void DeclencherDiapo();
};

#endif // MAINWINDOW_H
