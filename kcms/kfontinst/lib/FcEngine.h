#pragma once

/*
 * SPDX-FileCopyrightText: 2003-2007 Craig Drummond <craig@kde.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "KfiConstants.h"
#include <QColor>
#include <QFont>
#include <QRect>
#include <QVector>
#include <fontconfig/fontconfig.h>
//#include "Misc.h"
#include "Fc.h"

// Enable the following to use locale aware family name - if font supports this.
//#define KFI_USE_TRANSLATED_FAMILY_NAME

class KConfig;

typedef struct _XftFont XftFont;
typedef struct _XftDraw XftDraw;
typedef struct _XftColor XftColor;

namespace KFI
{
class Q_DECL_EXPORT CFcEngine
{
public:
    class Xft;

    struct TRange {
        TRange(quint32 f = 0, quint32 t = 0)
            : from(f)
            , to(t)
        {
        }
        bool null() const
        {
            return 0 == from && 0 == to;
        }

        quint32 from, to;
    };

    struct TChar : public QRect {
        TChar(const QRect &r = QRect(), quint32 u = 0)
            : QRect(r)
            , ucs4(u)
        {
        }

        quint32 ucs4;
    };

    static CFcEngine *instance();

    CFcEngine(bool init = true);
    virtual ~CFcEngine();

    void readConfig(KConfig &cfg);
    void writeConfig(KConfig &cfg);
    static void setDirty()
    {
        theirFcDirty = true;
    }
    QImage drawPreview(const QString &name, quint32 style, int faceNo, const QColor &txt, const QColor &bgnd, int h);
    QImage draw(const QString &name, quint32 style, int faceNo, const QColor &txt, const QColor &bgnd, int fSize, const QString &text);
    QImage draw(const QString &name,
                quint32 style,
                int faceNo,
                const QColor &txt,
                const QColor &bgnd,
                int w,
                int h,
                bool thumb,
                const QList<TRange> &range = QList<TRange>(),
                QList<TChar> *chars = nullptr);
    int getNumIndexes()
    {
        return itsIndexCount;
    } // Only valid after draw has been called!
    static QFont getQFont(const QString &family, quint32 style, int size);
    const QVector<int> &sizes() const
    {
        return itsSizes;
    }
    bool atMin() const
    {
        return 0 == itsSizes.size() || 0 == itsAlphaSizeIndex;
    }
    bool atMax() const
    {
        return 0 == itsSizes.size() || itsSizes.size() - 1 == itsAlphaSizeIndex;
    }
    void zoomIn()
    {
        if (!atMax())
            itsAlphaSizeIndex++;
    }
    void zoomOut()
    {
        if (!atMin())
            itsAlphaSizeIndex--;
    }
    int alphaSize() const
    {
        return itsSizes[itsAlphaSizeIndex];
    }
    quint32 styleVal()
    {
        return itsStyle;
    }
    const QString &descriptiveName() const
    {
        return itsDescriptiveName;
    }

    const QString &getPreviewString()
    {
        return itsPreviewString;
    }
    static QString getDefaultPreviewString();
    void setPreviewString(const QString &str)
    {
        itsPreviewString = str.isEmpty() ? getDefaultPreviewString() : str;
    }
    static QString getUppercaseLetters();
    static QString getLowercaseLetters();
    static QString getPunctuation();

    static const int constScalableSizes[];
    static const int constDefaultAlphaSize;

private:
    bool parse(const QString &name, quint32 style, int faceNo);
    XftFont *queryFont();
    XftFont *getFont(int size);
    bool isCorrect(XftFont *f, bool checkFamily);
    void getSizes();
    void drawName(int x, int &y, int h);
    void addFontFile(const QString &file);
    void reinit();
    Xft *xft();

private:
    bool itsInstalled;
    QString itsName, itsDescriptiveName;
    quint32 itsStyle;
    int itsIndex, itsIndexCount, itsAlphaSizeIndex;
    QVector<int> itsSizes;
    FcBool itsScalable;
    QStringList itsAddedFiles;
    QString itsPreviewString;
    static bool theirFcDirty;
    Xft *itsXft;
};

}
