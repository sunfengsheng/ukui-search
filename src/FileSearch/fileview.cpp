#include "fileview.h"
#include <QDebug>
#include <QApplication>

#define ORG_UKUI_STYLE            "org.ukui.style"
#define STYLE_NAME                "styleName"
#define STYLE_NAME_KEY_DARK       "ukui-dark"
#define STYLE_NAME_KEY_DEFAULT    "ukui-default"
#define STYLE_NAME_KEY_BLACK       "ukui-black"
#define STYLE_NAME_KEY_LIGHT       "ukui-light"
#define STYLE_NAME_KEY_WHITE       "ukui-white"


fileview::fileview(QWidget *parent)
{
    m_oldRow =0;
    this->setParent(parent);
    this->setFrameShape(QFrame::NoFrame);
    this->setFocusPolicy(Qt::TabFocus);
    const QByteArray style_id(ORG_UKUI_STYLE);

    /*
     * ukui-defalt 主题时，应用为黑色模式或白色模式取决于设计稿
     * 例如 ukui-panel,ukui-menu,ukui-sidebar 等应用为黑色模式
    */
    stylelist<<STYLE_NAME_KEY_DARK<<STYLE_NAME_KEY_BLACK;
    if(QGSettings::isSchemaInstalled(style_id)){
        style_settings = new QGSettings(style_id);
        styleChange();
    }

    connect(style_settings, &QGSettings::changed, this, [=] (const QString &key){
        if(key==STYLE_NAME){
            styleChange();
        }
    });
    this->setHeaderHidden(true);
    this->setContentsMargins(0,0,0,0);
}


/**
 * @brief fileview::~fileview 析构函数 delete指针
 */
fileview::~fileview()
{
    delete style_settings;
}


/**
 * @brief fileview::styleChange  用qss绘制两套QTreeView的样式
 */
void fileview::styleChange()
{
    if(stylelist.contains(style_settings->get(STYLE_NAME).toString())){
        //黑色主题下需要进行的处理
        setStyleSheet(
                    "QTreeView{font: bold;"
                    "font-size:20px;color: rgb(255, 255, 255);"
                    "background-color: black;"
                    "background:transparent}"
                    );
    }else{
        //白色主题下需要进行的处理
        setStyleSheet(
                    "QTreeView{font: bold;"
                    "font-size:20px;color: rgb(0, 0, 0);"
                    "background-color: white;"
                    "background:transparent}"
//                  "QTreeView::item:hover{background-color:}"
                    );
    }
}

bool fileview::event(QEvent *event){
    QTreeView::event(event);
    return true;
}

void fileview::keyPressEvent(QKeyEvent *event){
    if(event->text()=="\r"){
        Q_EMIT open();
    }
    if(event->key()==Qt::Key_Down){
        int row = this->currentIndex().row();
        if(row==2){
            Q_EMIT viewSwitchDown();
            return;
        }
        if(row==0){
            if(!this->indexBelow(this->currentIndex()).isValid()){
                Q_EMIT viewNoMoreSwitchDown();
            }
        }else if(row==1){
            if(!this->indexBelow(this->currentIndex()).isValid()){
                Q_EMIT viewNoMoreSwitchDown();
            }
        }
    }
    QTreeView::keyPressEvent(event);
}
