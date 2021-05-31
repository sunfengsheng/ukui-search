#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QTreeView>
#include <QHeaderView>
#include <QGSettings>
#include <QEvent>
#include <QKeyEvent>

class fileview : public QTreeView
{
    Q_OBJECT
public:
    fileview(QWidget *parent=nullptr);
    ~fileview();
private:
    //控制style的gsetting
    QGSettings *style_settings;
    //用来判断style
    QStringList stylelist;
    int m_oldRow;
    bool m_keyPress;
    void styleChange();

protected:
    bool event(QEvent *event);
    void keyPressEvent(QKeyEvent *event);

Q_SIGNALS:
    void open();
    void viewSwitchDown();
    void viewNoMoreSwitchDown();

};

#endif // FILEVIEW_H
