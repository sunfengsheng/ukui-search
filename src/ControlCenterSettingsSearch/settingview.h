#ifndef SETTINGVIEW_H
#define SETTINGVIEW_H

#include <QTreeView>
#include "../Style/style.h"
#include <QDebug>
#include <QListView>
#include <QKeyEvent>

class settingview : public QTreeView
{
    Q_OBJECT
public:
    settingview();
    ~settingview();

public Q_SLOTS:
    void changesize(int row);

private:
    //控制style的gsetting
    QGSettings *style_settings;
    //用来判断style
    QStringList stylelist;
    void styleChange();

protected:
    void keyPressEvent(QKeyEvent *event);

Q_SIGNALS:
    void open();
    void viewSwitchDown();
    void viewSwitchUp();


};

#endif // SETTINGVIEW_H
