#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QMainWindow>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CHargerImages();

    /*s_TailleImageMax.setWidth(ui->imageLabel->width());
    s_TailleImageMax.setHeight(ui->imageLabel->height());*/
    s_TailleImageMax.setWidth(800);
    s_TailleImageMax.setHeight(600);
    this->m_TailleInterfaceDeBase.setWidth( this->width() - s_TailleImageMax.width());
    this->m_TailleInterfaceDeBase.setHeight( this->height() - s_TailleImageMax.height());

    //DeclencherDiapo();
    QObject::connect(ui->btnLancerDiaporama, SIGNAL(clicked()), this, SLOT(DeclencherDiapo()));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    /*Qt::Key_Left	0x01000012
Qt::Key_Up	0x01000013
Qt::Key_Right	0x01000014
Qt::Key_Down*/
    switch( event->key() )
    {
    case  Qt::Key_X : {
        // mettre en pause
        this->m_Pause = !this->m_Pause;
        if (!m_Pause)
            RafraichirAffichage();
    } break;

    case Qt::Key_C : {
        // passer à l'image suivante
        this->m_Pause = false;
        RafraichirAffichage();
    } break;
    }

    return QWidget::keyPressEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CHargerImages()
{
    //qDebug()<<QDir::absolutePath();
    //assume the directory exists and contains some files and you want all jpg and JPG files
    //QDir directory("D:/boulot/QtProjects/Diaporameur/images/");
    QDir directory(m_Dossier);
    m_ImagesFixes = directory.entryList(QStringList() << "*.jpg" << "*.JPG"<< "*.PNG"<< "*.png",QDir::Files);
    m_Gifs = directory.entryList(QStringList() << "*.gif" << "*.GIF",QDir::Files);

    QString texte = QString("Nb images : ") +
            QString::number(m_ImagesFixes.count()) +
            QString(" - nb gifs : ") +
            QString::number(m_Gifs.count());
    ui->texteImagesLabel->setText(texte);
}

void MainWindow::DeclencherDiapo()
{

    // lancer le timer :
    m_Timer = new QTimer(this);
    m_Timer->setSingleShot(true);
    //m_Timer->setSingleShot(true);
    connect(m_Timer, SIGNAL(timeout()), SLOT(RafraichirAffichage()));
    QLineEdit* input = static_cast<QLineEdit*>(ui->inputDureeDiapo);
    m_BaseDureeMs = (input->text().toInt());
    m_Timer->start(m_BaseDureeMs);

    // tout cacher :
    //ui->Interface->setMaximumHeight(0);
    ui->centralWidget->layout()->removeWidget(ui->Interface);
    ui->Interface->setVisible(false);
    ui->imageLabel->update();
    this->RecalculerTailleImageMax();
}

QString MainWindow::DeterminerImage(TypeImage typeImage)
{
    unsigned int val = QDateTime::currentDateTime ().toTime_t();
    qsrand(val);

    int v = qrand() % m_ImagesFixes.length();

    if ( typeImage == TypeImage::ImgAnim ) {
        v = qrand() % m_Gifs.length();
        return m_Dossier + m_ImagesFixes[v];
    } else if ( typeImage == TypeImage::Toutes ) {
        v = qrand() % ( m_Gifs.length() + m_ImagesFixes.length());
        if ( v > m_Gifs.length()) return m_Dossier + m_ImagesFixes[v - m_Gifs.length()];
        return m_Dossier + m_Gifs[v];
    }

    return m_Dossier + m_ImagesFixes[v];
}

//
void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   // Your code here.
   ui->centralWidget->resize(this->width(), this->height());

   this->RecalculerTailleImageMax();
}

void MainWindow::RecalculerTailleImageMax()
{
    int w = this->width();
    int h = this->height();
    s_TailleImageMax.setWidth(this->width() - m_TailleInterfaceDeBase.width());
    s_TailleImageMax.setHeight(this->height() - m_TailleInterfaceDeBase.height());
}

void MainWindow::RafraichirAffichage()
{
    if ( m_Pause ) return;

    bool afficheFilm = (ui->imageLabel->m_Film != nullptr);
    if ( afficheFilm ) {
        ui->imageLabel->m_Film->stop();
        delete ui->imageLabel->m_Film;
        ui->imageLabel->m_Film = nullptr;
    }

    bool afficheImg = (ui->imageLabel->m_Pixel != nullptr);
    if ( afficheImg ) {
        delete ui->imageLabel->m_Pixel;
        ui->imageLabel->m_Pixel = nullptr;
    }

    QString chemin = DeterminerImage(TypeImage::Toutes);

    ui->imageLabel->clear();

    if ( chemin != "" )
    {
        // si l'image est un gif je pars du principe que c'est un gif potentiellement animé et ça devient un movie au lieu d'une image
        if ( chemin.endsWith(".gif") || chemin.endsWith(".GIF"))
        {
            //ui->imageLabel->m_Pixel = nullptr;
            ui->imageLabel->show();
            ui->imageLabel->m_Film = new QMovie(chemin);
            ui->imageLabel->setMovie(ui->imageLabel->m_Film);
            ui->imageLabel->m_Film->start();
            ui->imageLabel->m_Film->jumpToNextFrame();

            //ui->imageLabel->setScaledContents(true);
            QSize my_size = QSize(ui->imageLabel->widthForHeight(s_TailleImageMax.height()), s_TailleImageMax.height());
            ui->imageLabel->m_Film->setScaledSize(my_size);

            int dureeBase = ui->imageLabel->m_Film->frameCount() * ui->imageLabel->m_Film->nextFrameDelay();
            int dureeFinale = dureeBase;
            // répétition du gif suffisament de fois pour atteindre le temps de base
            while (dureeFinale < m_BaseDureeMs) {
                dureeFinale += dureeBase;
            }

            m_Timer->start(dureeFinale);
        }
        else
        {
            ui->imageLabel->setScaledContents(false);
            ui->imageLabel->m_Film = nullptr;
            ui->imageLabel->show();
            QImage* img = new QImage();
            img->load(chemin);
            ui->imageLabel->setPixmap(QPixmap::fromImage(*img));



            // scale according to width or height ?
            /*int diffHeight = qAbs( s_TailleImageMax.height() - img->height());
            int diffWidth = qAbs( s_TailleImageMax.width() - img->width());
            if ( diffHeight > diffWidth ) {
                ui->imageLabel->setPixmap(QPixmap::fromImage(*img).scaledToHeight(s_TailleImageMax.height()));
            } else {
                ui->imageLabel->setPixmap(QPixmap::fromImage(*img).scaledToWidth(s_TailleImageMax.width()));
            }*/


            ui->imageLabel->setMaximumSize(s_TailleImageMax);
            //ui->imageLabel->setMinimumSize(s_TailleImageMax);
            ui->imageLabel->MajImageSize();

            m_Timer->start(m_BaseDureeMs);
        }
    }

}
