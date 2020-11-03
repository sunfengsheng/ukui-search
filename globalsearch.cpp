#include "globalsearch.h"
#include "ui_globalsearch.h"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include "QFile"
#include "QByteArray"
#include "QFileInfo"
#include <QMessageBox>
#include <pwd.h>
#include <unistd.h>


GlobalSearch::GlobalSearch(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GlobalSearch)
{
    ui->setupUi(this);
//    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);

    ui->toolButton->setStyle(new CustomStyle);
    ui->toolButton_2->setStyle(new CustomStyle);
    ui->toolButton_3->setStyle(new CustomStyle);
    ui->toolButton_4->setStyle(new CustomStyle);
}

GlobalSearch::~GlobalSearch()
{
    delete ui;
}


bool GlobalSearch::eventFilter(QObject *watched, QEvent *evt)
{
//    static int index = 1;
//    static QPoint mousePoint;
//    static bool mousePressed = false;

//    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
//    if (event->type() == QEvent::MouseButtonPress) {
//        if (event->button() == Qt::LeftButton) {
//            mousePressed = true;
//            mousePoint = event->globalPos() - this->pos();

//            if (index == 5) {
//                index = 1;
//            } else {
//                index++;
//            }

////            ui->widget->setStyleSheet(QString("background-image:url(:/image/%1.png);").arg(index));

//            return true;
//        } else {
//            exit(0);
//        }
//    } else if (event->type() == QEvent::MouseButtonRelease) {
//        mousePressed = false;
//        return true;
//    } else if (event->type() == QEvent::MouseMove) {
//        if (mousePressed && (event->buttons() && Qt::LeftButton)) {
//            this->move(event->globalPos() - mousePoint);
//            return true;
//        }
//    }

//    return QWidget::eventFilter(watched, event);
}


void GlobalSearch::on_pushButton_clicked()
{
    qDebug()<<""<<ui->lineEdit->text();
    showText();

}

void GlobalSearch::showText()
{
    ui->toolButton->setText(QString("在浏览器搜索：%1").arg(ui->lineEdit->text()));
    ui->toolButton_2->setText(QString("在文件管理器中搜索：%1").arg(ui->lineEdit->text()));
//    ui->toolButton_3->setText(QString("打开应用程序：%1").arg(ui->lineEdit->text()));
    searchApplication();
    ui->toolButton_4->setText(QString("到应用商店搜索：%1").arg(ui->lineEdit->text()));

}

void GlobalSearch::on_toolButton_clicked()
{
    QDesktopServices::openUrl(QUrl(QString("https://www.baidu.com/s?wd=%1").arg(ui->lineEdit->text())));
}

void GlobalSearch::on_toolButton_2_clicked()
{

    //ui->toolButton_2->setText(QString("在文件管理器中搜索：%1").arg(ui->lineEdit->text()));
    struct passwd *pwd;
    pwd=getpwuid(getuid());
    qDebug()<<"user name"<<pwd->pw_name;
    QString str = QString("search:///search_uris=file:///home/%1&name_regexp=%2&recursive=1").arg(pwd->pw_name).arg(ui->lineEdit->text());
    QProcess p;
    p.setProgram(QString("peony"));
    p.setArguments(QStringList()<<str);
    p.startDetached(p.program(), p.arguments());
    p.waitForFinished(-1);

}

void GlobalSearch::showApplication()
{
//    ui->toolButton_3->setText(QString("打开应用程序：%1").arg(ui->lineEdit->text()));
//    ui->toolButton_4->setText(QString("到应用商店搜索：%1").arg(ui->lineEdit->text()));
}

void GlobalSearch::searchApplication()
{
    desktopConver(ui->lineEdit->text());
}

/*
 * 将进程名转化为对应的desktop文件，然后通过desktopToName将desktop文件中的Name[zh_CN]字段提取
*/
void GlobalSearch::desktopConver(QString process)
{
    if(isFileExist("/etc/xdg/autostart/"+process+".desktop") || isFileExist("/usr/share/applications/"+process+".desktop")){
        if(isFileExist("/etc/xdg/autostart/"+process+".desktop")){
            desktopToName("/etc/xdg/autostart/"+process+".desktop");
        }else{
            desktopToName("/usr/share/applications/"+process+".desktop");
        }
    }
    else{
        cmd = new QProcess(this);
        connect(cmd , SIGNAL(readyReadStandardOutput()) , this , SLOT(on_readoutput()));
        /*异常处理*/
        //connect(cmd , SIGNAL(readyReadStandardError()) , this , SLOT(on_readerror()));

        cmd->start("bash");             //启动终端(Windows下改为cmd)
        cmd->waitForStarted();          //等待启动完成
        QString input_cmd;
        input_cmd=QString("grep -nr %1 /usr/share/applications/  /etc/xdg/autostart/\n").arg(ui->lineEdit->text());
        cmd->write("dpkg --print-architecture\n");
//        cmd->write(input_cmd.toUtf8());               //向终端写入“ls”命令，注意尾部的“\n”不可省略
    }
}

bool GlobalSearch::isFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}

/*
 * 将desktop文件中Name[zh_CN]字段提取
 * 若Name[zh_CN]字段为英文则提取GenericName[zh_CN]
 * （以fcitx为例，fcitx.desktop的   Name[zh_CN]=fcitx，GenericName[zh_CN]=输入法）
 * 以desktop文件的规范性来讲，其实不应处理此逻辑，GenericName[zh_CN]字段本就不是应用的汉化名称，而是应用的类型通用名称，
 * 在同类型应用较为丰富的情况下，对GenericName[zh_CN]进行处理必然导致歧义
 * 自研托盘应用应尽量遵循desktop文件的规范，Name[zh_CN]字段为中文，GenericName[zh_CN]字段应写此类应用的通用中文名称
*/
QString GlobalSearch::desktopToName(QString desktopfile)
{
    QFile file(desktopfile);
    bool flag=false;
    qDebug()<<"desktop name   :  "<<desktopfile;
    if(!isFileExist(desktopfile)){
        qDebug()<<"Nodesktop file ;No Chinese name";
        return "";
    }
    if (!file.open(QIODevice::ReadOnly)) qDebug() << "Read DesktopFile Failed.";
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        if (str.contains("Name[zh_CN]") && str.contains(QRegExp("[\\x4e00-\\x9fa5]+"))){
            if(!str.contains("GenericName")){
                str=str.remove("Name[zh_CN]=");
            }else{
                str=str.remove("GenericName[zh_CN]=");
            }
            str=str.remove("\n");
            ui->toolButton_3->setText(QString("打开应用程序：%1").arg(str));
            qDebug()<<"Chinese name: "<<str;
            flag=true;
            return str;
        }
        if(file.atEnd() && !flag){
            qDebug()<<"No Chinese name";
            //可用.isEmpty()判断是否返回值为空；不可用 .isNull();
            return "";
        }
    }
}

/*
 * QString("grep -nr %1 /usr/share/applications/  /etc/xdg/autostart/\n").arg(ui->lineEdit->text());
 * 遍历 /usr/share/applications/  /etc/xdg/autostart/  目录寻找匹配的desktop文件
*/
void GlobalSearch::on_readoutput()
{
    QFile file("/tmp/desktopprocess.txt");
    QString output=cmd->readAllStandardOutput().data();
    qDebug()<<"  output   :"<<output;

    //打开文件，不存在则创建
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.write(output.toUtf8());
    file.close();
//    qDebug()<<output.toUtf8();


    file.open(QIODevice::ReadWrite | QIODevice::Text);
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        if(str.contains(".desktop:") && str.contains(":Exec")){
            str=str.section(".desktop",0,0)+".desktop";
            ui->toolButton_3->setText(QString("打开应用程序：%1").arg(str));
            desktopToName(str);
        }
    }
    file.close();
    file.remove();
}

void GlobalSearch::on_readerror()
{
    QMessageBox::information(0, "Error", cmd->readAllStandardError().data());    //弹出信息框提示错误信息
}

void GlobalSearch::on_toolButton_3_clicked()
{
    QProcess app;
    app.startDetached(ui->lineEdit->text());
}

void GlobalSearch::on_lineEdit_textChanged(const QString &arg1)
{
    showText();
}
