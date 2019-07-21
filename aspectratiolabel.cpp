#include "aspectratiolabel.h"
#include <QDebug>

AspectRatioLabel::AspectRatioLabel(QWidget* parent, Qt::WindowFlags f) : QLabel(parent, f)
{
}

AspectRatioLabel::~AspectRatioLabel()
{
}

void AspectRatioLabel::setPixmap(const QPixmap& pm)
{
    delete m_Pixel;
    m_Pixel = new QPixmap(pm);

    updateMargins();
    QLabel::setPixmap(*m_Pixel);
}

void AspectRatioLabel::setMovie(QMovie* mv)
{
    //delete m_Film;
    //m_Film = new QMovie(mv);

    updateMargins();
    QLabel::setMovie(mv);
}

int AspectRatioLabel::heightForWidth( int width ) const
{
    int hauteurFinale = qRound(static_cast<float>(m_Pixel->height()*width)/m_Pixel->width());
    return m_Pixel->isNull() ? this->height() : hauteurFinale;
}

int AspectRatioLabel::widthForHeight( int hauteurFinale ) const
{
    int largeurFinale = -1;
    if ( m_Pixel != nullptr ) {
        largeurFinale = qRound(static_cast<float>(m_Pixel->width()*hauteurFinale)/m_Pixel->height());
    } else if ( m_Film != nullptr ) {
        m_Film->jumpToFrame(0);
        largeurFinale = qRound(static_cast<float>(m_Film->currentImage().width()*hauteurFinale)/m_Film->currentImage().height());
    }

    return largeurFinale;
}

QSize AspectRatioLabel::sizeHint() const
{
    //int h = this->maximumHeight();
    int h = this->height();
    return QSize( this->widthForHeight(h), h );
}

QPixmap AspectRatioLabel::scaledPixmap() const
{
    return m_Pixel->scaled(this->maximumSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void AspectRatioLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

void AspectRatioLabel::resizeEvent(QResizeEvent* event)
{
    updateMargins();

    if(m_Pixel != nullptr)
        this->MajImageSize();

    QLabel::resizeEvent(event);
}

void AspectRatioLabel::MajImageSize()
{
    if ( m_Pixel != nullptr)
        QLabel::setPixmap(scaledPixmap());
}

void AspectRatioLabel::updateMargins()
{
    int w = this->width();
    int h = this->height();

    if (w <= 0 || h <= 0)
        return;

    setContentsMargins(0, 0, 0, 0);

    /*if (w * pixmapHeight > h * pixmapWidth)
    {
        int m = (w - (pixmapWidth * h / pixmapHeight)) / 2;
        setContentsMargins(m, 0, m, 0);
    }
    else
    {
        int m = (h - (pixmapHeight * w / pixmapWidth)) / 2;
        setContentsMargins(0, m, 0, m);
    }*/
}
