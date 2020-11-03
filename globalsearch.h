#ifndef GLOBALSEARCH_H
#define GLOBALSEARCH_H

#include <QWidget>
#include <QProcess>

#include "customstyle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GlobalSearch; }
QT_END_NAMESPACE

class GlobalSearch : public QWidget
{
    Q_OBJECT

public:
    GlobalSearch(QWidget *parent = nullptr);
    ~GlobalSearch();

protected:
    bool eventFilter(QObject *watched, QEvent *evt);

private slots:
    void on_pushButton_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void showApplication();

    void showText();

    void searchApplication();

    void desktopConver(QString process);

    bool isFileExist(QString fullFileName);
    QString desktopToName(QString desktopfile);
    void on_readoutput();
    void on_readerror();

    void on_toolButton_3_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::GlobalSearch *ui;

    QProcess *cmd;
};
#endif // GLOBALSEARCH_H
