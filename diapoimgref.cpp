#include "diapoimgref.h"
#include <QDebug>
#include <QFile>
#include "mainwindow.h"

DiapoImgRef::DiapoImgRef(TypeImage typeImg, QString chemin):m_TypeImg(typeImg), m_Chemin(chemin), m_PhaseImage(PhaseImage::Inconnu)
{
    QFile file( GetCheminCompletFichierBibli() );
    if(file.open(QIODevice::ReadOnly)) {

        QTextStream in(&file);

        while(!in.atEnd()) {
            QString line = in.readLine();
            m_PhaseImage = static_cast<PhaseImage>(line.toInt());
        }

        file.close();
    }
}

QString DiapoImgRef::GetCheminCompletImg()
{
    return MainWindow::DOSSIER + this->m_Chemin;
}

QString DiapoImgRef::GetCheminCompletFichierBibli()
{
    return MainWindow::DOSSIER + this->m_Chemin+ ".diap";
}

void DiapoImgRef::SetPhase(PhaseImage phase)
{
    m_PhaseImage = phase;

    QFile file( GetCheminCompletFichierBibli() );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << phase << endl;
    }

    qDebug() << "SetPhase type image : " << phase << endl;
}
