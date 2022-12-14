#pragma once

/*
 * SPDX-FileCopyrightText: 2003-2007 Craig Drummond <craig@kde.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "JobRunner.h"
#include "Misc.h"
#include <QDialog>
#include <QThread>
#include <QTreeWidget>

class QLabel;
class QMenu;
class QAction;
class QDialogButtonBox;
class QAbstractButton;

namespace KFI
{
class CActionLabel;
class CFontList;
class CDuplicatesDialog;

class CFontFileList : public QThread
{
    Q_OBJECT

public:
    typedef QHash<Misc::TFont, QSet<QString>> TFontMap;

    //
    // TFile store link from filename to FontMap item.
    // This is used when looking for duplicate filenames (a.ttf/a.TTF).
    struct TFile {
        TFile(const QString &n, CFontFileList::TFontMap::Iterator i)
            : name(n)
            , it(i)
            , useLower(false)
        {
        }
        TFile(const QString &n, bool l = false)
            : name(n)
            , useLower(l)
        {
        }

        bool operator==(const TFile &f) const
        {
            return useLower || f.useLower ? name.toLower() == f.name.toLower() : name == f.name;
        }

        QString name;
        CFontFileList::TFontMap::Iterator it;
        bool useLower;
    };

public:
    CFontFileList(CDuplicatesDialog *parent);

    void start();
    void terminate();
    void getDuplicateFonts(TFontMap &map);
    bool wasTerminated() const
    {
        return itsTerminated;
    }

Q_SIGNALS:

    void finished();

private:
    void run() override;
    void fileDuplicates(const QString &folder, const QSet<TFile> &files);

private:
    bool itsTerminated;
    TFontMap itsMap;
};

class CFontFileListView : public QTreeWidget
{
    Q_OBJECT

public:
    class StyleItem : public QTreeWidgetItem
    {
    public:
        StyleItem(CFontFileListView *parent, const QStringList &details, const QString &fam, quint32 val)
            : QTreeWidgetItem(parent, details)
            , itsFamily(fam)
            , itsValue(val)
        {
        }

        const QString &family() const
        {
            return itsFamily;
        }
        quint32 value() const
        {
            return itsValue;
        }

    private:
        QString itsFamily;
        quint32 itsValue;
    };

    CFontFileListView(QWidget *parent);
    ~CFontFileListView() override
    {
    }

    QSet<QString> getMarkedFiles();
    CJobRunner::ItemList getMarkedItems();
    void removeFiles();

Q_SIGNALS:

    void haveDeletions(bool have);

private Q_SLOTS:

    void openViewer();
    void properties();
    void mark();
    void unmark();
    void selectionChanged();
    void clicked(QTreeWidgetItem *item, int col);
    void contextMenuEvent(QContextMenuEvent *ev) override;

private:
    void checkFiles();

private:
    QMenu *itsMenu;
    QAction *itsMarkAct, *itsUnMarkAct;
};

class CDuplicatesDialog : public QDialog
{
    Q_OBJECT

public:
    CDuplicatesDialog(QWidget *parent, CFontList *fl);

    int exec() override;
    const CFontList *fontList() const
    {
        return itsFontList;
    }

private Q_SLOTS:

    void scanFinished();
    void slotButtonClicked(QAbstractButton *button);
    void enableButtonOk(bool);

private:
    QDialogButtonBox *itsButtonBox;
    CActionLabel *itsActionLabel;
    CFontFileList *itsFontFileList;
    QLabel *itsLabel;
    CFontFileListView *itsView;
    CFontList *itsFontList;
};

}
