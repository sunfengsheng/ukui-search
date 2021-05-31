#ifndef MOREBUTTON_H
#define MOREBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QKeyEvent>

class MoreButton : public QPushButton
{
    Q_OBJECT
public:
    MoreButton(QWidget *parent);
    void setText(QString text);

private:
    void InitUi();

private:
    QLabel *m_Textlabel;
    QLabel *m_Iconlabel;
    QHBoxLayout *m_Layout;

protected:
    void keyPressEvent(QKeyEvent *event);

Q_SIGNALS:
    void open();
    void switchUpModule();
    void switchDownModule();


};
#endif // MOREBUTTON_H
