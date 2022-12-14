/*
    SPDX-FileCopyrightText: 2003-2007 Craig Drummond <craig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "FontPreview.h"
#include "CharTip.h"
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <stdlib.h>

namespace KFI
{
static const int constBorder = 4;
static const int constStepSize = 16;

CFontPreview::CFontPreview(QWidget *parent)
    : QWidget(parent)
    , itsCurrentFace(1)
    , itsLastWidth(0)
    , itsLastHeight(0)
    , itsStyleInfo(KFI_NO_STYLE_INFO)
    , itsTip(nullptr)
{
    itsEngine = new CFcEngine;
}

CFontPreview::~CFontPreview()
{
    delete itsTip;
    delete itsEngine;
}

void CFontPreview::showFont(const QString &name, unsigned long styleInfo, int face)
{
    itsFontName = name;
    itsStyleInfo = styleInfo;
    showFace(face);
}

void CFontPreview::showFace(int face)
{
    itsCurrentFace = face;
    showFont();
}

void CFontPreview::showFont()
{
    itsLastWidth = width() + constStepSize;
    itsLastHeight = height() + constStepSize;

    itsImage = itsEngine->draw(itsFontName,
                               itsStyleInfo,
                               itsCurrentFace,
                               palette().text().color(),
                               palette().base().color(),
                               itsLastWidth,
                               itsLastHeight,
                               false,
                               itsRange,
                               &itsChars);

    if (!itsImage.isNull()) {
        itsLastChar = CFcEngine::TChar();
        setMouseTracking(itsChars.count() > 0);
        update();
        emit status(true);
        emit atMax(itsEngine->atMax());
        emit atMin(itsEngine->atMin());
    } else {
        itsLastChar = CFcEngine::TChar();
        setMouseTracking(false);
        update();
        emit status(false);
        emit atMax(true);
        emit atMin(true);
    }
}

void CFontPreview::zoomIn()
{
    itsEngine->zoomIn();
    showFont();
    emit atMax(itsEngine->atMax());
}

void CFontPreview::zoomOut()
{
    itsEngine->zoomOut();
    showFont();
    emit atMin(itsEngine->atMin());
}

void CFontPreview::setUnicodeRange(const QList<CFcEngine::TRange> &r)
{
    itsRange = r;
    showFont();
}

void CFontPreview::paintEvent(QPaintEvent *)
{
    QPainter paint(this);

    paint.fillRect(rect(), palette().base());
    if (!itsImage.isNull()) {
        if (abs(width() - itsLastWidth) > constStepSize || abs(height() - itsLastHeight) > constStepSize) {
            showFont();
        } else {
            paint.drawImage(
                QPointF(constBorder, constBorder),
                itsImage,
                QRectF(0, 0, (width() - (constBorder * 2)) * itsImage.devicePixelRatioF(), (height() - (constBorder * 2)) * itsImage.devicePixelRatioF()));
        }
    }
}

void CFontPreview::mouseMoveEvent(QMouseEvent *event)
{
    if (!itsChars.isEmpty()) {
        QList<CFcEngine::TChar>::ConstIterator end(itsChars.end());

        if (itsLastChar.isNull() || !itsLastChar.contains(event->pos())) {
            for (QList<CFcEngine::TChar>::ConstIterator it(itsChars.begin()); it != end; ++it) {
                if ((*it).contains(event->pos())) {
                    if (!itsTip) {
                        itsTip = new CCharTip(this);
                    }

                    itsTip->setItem(*it);
                    itsLastChar = *it;
                    break;
                }
            }
        }
    }
}

void CFontPreview::wheelEvent(QWheelEvent *e)
{
    if (e->angleDelta().y() > 0) {
        zoomIn();
    } else if (e->angleDelta().y() < 0) {
        zoomOut();
    }

    e->accept();
}

QSize CFontPreview::sizeHint() const
{
    return QSize(132, 132);
}

QSize CFontPreview::minimumSizeHint() const
{
    return QSize(32, 32);
}

}
