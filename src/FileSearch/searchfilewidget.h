#ifndef SEARCHFILEWIDGET_H
#define SEARCHFILEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QtDBus/QtDBus>
#include <QDebug>
#include <QString>
#include <QPainter>
#include "fileview.h"
#include "filemodel.h"
#include "searchfilethread.h"

class SearchFileWidget : public QWidget
{
    Q_OBJECT
public:
     SearchFileWidget();
     QLabel *fileHead;
     fileview *fileView;
     filemodel *m_filemodel;//文件model
     QVBoxLayout *m_listLayout;
     SearchFileThread *m_searchFileThread=nullptr;
private:
     void initFilesearchUI();
     void paintEvent(QPaintEvent *e);


private Q_SLOTS:
     void fileTextRefresh(QString);
     void recvFileSearchResult(QStringList arg);

Q_SIGNALS:
     void sendSearchKeyword(QString arg);


};

#endif // SEARCHFILEWIDGET_H
