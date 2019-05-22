#ifndef ASPECTRATIOPIXMAPLABEL_H
#define ASPECTRATIOPIXMAPLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>
#include <QMovie>

class AspectRatioLabel : public QLabel
{
    Q_OBJECT
public:
    explicit AspectRatioLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~AspectRatioLabel() override;
    QPixmap scaledPixmap() const;
    int heightForWidth( int width ) const override;
    int widthForHeight( int height ) const;
    QSize sizeHint() const override;
    void MajImageSize();

    QPixmap* m_Pixel = nullptr;
    QMovie* m_Film = nullptr;

public slots:
    void setPixmap(const QPixmap& pm);
    void setMovie(QMovie* mv);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateMargins();
};

#endif // ASPECTRATIOPIXMAPLABEL_H
