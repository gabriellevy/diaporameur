#ifndef DIAPOIMGREF_H
#define DIAPOIMGREF_H

#include <QString>

enum TypeImage {
    ImgFixe,
    ImgAnim,
    Toutes
};

enum PhaseImage {
    Intro, // image devant être jouée de préférence au début du diaporama
    Courant,
    Final, // image devant être affichée plutôt à la fin du diaporama
    Inconnu // pas encore déterminé par l'utilisateur
};

/**
 * @brief classe qui ne contient pas l'image en elle-même mais ses références ce qui permet de la charger (et de la décrire)
 */
class DiapoImgRef
{
public:
    DiapoImgRef(TypeImage typeImg, QString chemin);

    QString m_Chemin;
    TypeImage m_TypeImg;
    PhaseImage m_PhaseImage;

    void SetPhase(PhaseImage phase);
    QString GetCheminComplet();
};

#endif // DIAPOIMGREF_H
