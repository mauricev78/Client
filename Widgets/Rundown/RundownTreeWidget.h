#pragma once

#include "../Shared.h"
#include "ui_RundownTreeWidget.h"

#include "Global.h"

#include "GpiDevice.h"

#include "Animations/BorderAnimation.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QEvent>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

#include <QtGui/QKeyEvent>
#include <QtGui/QMenu>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownTreeWidget : public QWidget, Ui::RundownTreeWidget
{
    Q_OBJECT

    public:
        explicit RundownTreeWidget(QWidget* parent = 0);

        void setActive(bool active);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        QString page;
        QString currentFilename;

        QMenu* contextMenuNew;
        QMenu* contextMenuColor;
        QMenu* contextMenuMixer;
        QMenu* contextMenuLibrary;
        QMenu* contextMenuRundown;

        bool active;
        bool compactView;
        bool enterPressed;

        BorderAnimation* rundownAnimation;

        QMap<int, Playout::PlayoutType::Type> gpiBindings;

        QTreeWidgetItem* copyItem;

        enum ActionSource
        {
            KeyPress,
            GpiPulse
        };

        void setupMenus();
        void checkEmptyRundown();
        void colorizeItems(const QString& color);
        bool executeCommand(Playout::PlayoutType::Type type, ActionSource source);
        bool groupItems();
        bool moveItemDown();
        bool moveItemIntoGroup();
        bool moveItemOutOfGroup();
        bool moveItemUp();
        void parsePage(QKeyEvent* event);
        bool ungroupItems();
        bool copySelectedItem();
        bool pasteSelectedItem();
        bool duplicateSelectedItem();
        void readRundownGroup(const QString& type, boost::property_tree::wptree& pt);
        void readRundownItem(const QString& type, boost::property_tree::wptree& pt, QTreeWidgetItem* parent);
        void writeRundownGroup(const QString& type, QXmlStreamWriter* writer, QTreeWidgetItem* child);
        void writeRundownItem(const QString& type, QXmlStreamWriter* writer, QTreeWidgetItem* item);

        Q_SLOT void addPrintItem();
        Q_SLOT void addSeparatorItem();
        Q_SLOT void addFileRecorderItem();
        Q_SLOT void addImageScrollerItem();
        Q_SLOT void addAudioItem();
        Q_SLOT void addImageItem();
        Q_SLOT void addTemplateItem();
        Q_SLOT void addVideoItem();
        Q_SLOT void addBlendModeItem();
        Q_SLOT void addSolidColorItem();
        Q_SLOT void addBrightnessItem();
        Q_SLOT void addCommitItem();
        Q_SLOT void addContrastItem();
        Q_SLOT void addCropItem();
        Q_SLOT void addDeckLinkInputItem();
        Q_SLOT void addGeometryItem();
        Q_SLOT void addGpiOutputItem();
        Q_SLOT void addGridItem();
        Q_SLOT void addClearOutputItem();
        Q_SLOT void addKeyerItem();
        Q_SLOT void addLevelsItem();
        Q_SLOT void addOpacityItem();
        Q_SLOT void addSaturationItem();
        Q_SLOT void addVolumeItem();
        Q_SLOT void contextMenuColorTriggered(QAction*);
        Q_SLOT void contextMenuRundownTriggered(QAction*);
        Q_SLOT void customContextMenuRequested(const QPoint&);
        Q_SLOT void gpiPortTriggered(int, GpiDevice*);
        Q_SLOT void gpiBindingChanged(int, Playout::PlayoutType::Type);
        Q_SLOT void currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);
        Q_SLOT void itemDoubleClicked(QTreeWidgetItem*, int);
        Q_SLOT void itemClicked(QTreeWidgetItem*, int);
        Q_SLOT void contextMenuNewTriggered(QAction*);
        Q_SLOT void selectItemBelow();
        Q_SLOT bool removeSelectedItems();
};