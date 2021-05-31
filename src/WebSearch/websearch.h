#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include <QPushButton>
#include <QtDBus/QtDBus>
#include <QString>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>
#include <QKeyEvent>
class websearch;
class webwidget : public QWidget
{
    Q_OBJECT
public:
    webwidget();
    websearch *web;
protected:
    void paintEvent(QPaintEvent *e);
private Q_SLOTS:
    void webSearchTextRefresh(QString mSearchText);

};

class websearch : public QPushButton
{
    Q_OBJECT
public:
     websearch(QWidget *parent=nullptr);
     QString searchString;
     QString search;

private:
     QLabel *m_buttonText;
     QHBoxLayout *m_buttonLayout;
     QLabel *m_Iconlabel;

protected:
    void keyPressEvent(QKeyEvent *event);

private Q_SLOTS:
     void webSearchTextRefresh(QString);

Q_SIGNALS:
     void viewSwitchUp();

};

#endif
