#include "kis_color_label_button.h"

#include <QStylePainter>
#include <QStyleOption>
#include <QMimeData>

#include "kis_global.h"
#include "kis_debug.h"

struct KisColorLabelButton::Private {
    QColor color;
};

KisColorLabelButton::KisColorLabelButton(QColor color, QWidget *parent) : QAbstractButton(parent), m_d(new Private())
{
    setCheckable(true);
    setAcceptDrops(true);
    setChecked(true);
    m_d->color = color;
}

KisColorLabelButton::~KisColorLabelButton()
{

}

void KisColorLabelButton::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QStylePainter painter(this);
    QStyleOption styleOption;
    styleOption.initFrom(this);

    if (isDown() || isChecked()){
        styleOption.state |= QStyle::State_On;
    }

    // Draw fill..
    QRect fillRect = kisGrowRect(rect(), -2);
    fillRect.width();
    if (m_d->color.alpha() > 0) {
        QColor fillColor = m_d->color;

        if (!isChecked()) {
            fillColor.setAlpha(64);
        }

        painter.fillRect(fillRect, fillColor);
    } else {
        // draw an X for no color for the first item
        int shortestEdge = std::min(fillRect.width(), fillRect.height());
        bool horizontal = fillRect.width() > fillRect.height();
        QRect srcRect = horizontal ? fillRect.adjusted(shortestEdge, 0, -shortestEdge, 0) : fillRect.adjusted(0, shortestEdge, 0, -shortestEdge);
        QRect crossRect = kisGrowRect(srcRect, -1);

        QColor shade = styleOption.palette.text().color();

        if (!isChecked()) {
            shade.setAlpha(64);
        }

        painter.setPen(QPen(shade, 2));
        painter.drawLine(crossRect.topLeft(), crossRect.bottomRight());
        painter.drawLine(crossRect.bottomLeft(), crossRect.topRight());
    }
}

QSize KisColorLabelButton::sizeHint() const
{
    return QSize(16,32);
}
