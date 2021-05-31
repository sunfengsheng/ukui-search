#include "appview.h"

#define ORG_UKUI_STYLE            "org.ukui.style"
#define STYLE_NAME                "styleName"
#define STYLE_NAME_KEY_DARK       "ukui-dark"
#define STYLE_NAME_KEY_DEFAULT    "ukui-default"
#define STYLE_NAME_KEY_BLACK       "ukui-black"
#define STYLE_NAME_KEY_LIGHT       "ukui-light"
#define STYLE_NAME_KEY_WHITE       "ukui-white"

/**
 * @brief settingview::settingview 控制面板搜索项的QTreeView
 */
appview::appview()
{

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

}

/**
 * @brief settingview::~settingview 析构函数 delete指针
 */
appview::~appview()
{
    delete style_settings;
}

/**
 * @brief settingview::changesize 动态改变控制面板搜索结果界面的高度
 * @param row
 */
void appview::changesize(int row)
{
    if(row<3){
        this->setFixedSize(Style::defaultMainViewWidWidth,row*28+30);
    } else {
        this->setFixedSize(Style::defaultMainViewWidWidth,3*28+30);
    }

}

/**
 * @brief settingview::styleChange  用qss绘制两套QTreeView的样式
 */
void appview::styleChange()
{
    if(stylelist.contains(style_settings->get(STYLE_NAME).toString())){
        //黑色主题下需要进行的处理
        setStyleSheet(
                    "font: bold; "
                    "font-size:20px; "
                    "color: rgb(255, 255, 255); "
                    "background-color: green; "
                    "background:transparent"
                    );
    }else{
        //白色主题下需要进行的处理
        setStyleSheet(
                    "font: bold; "
                    "font-size:20px; "
                    "color: rgb(0, 0, 0); "
                    "background-color: green; "
                    "background:transparent"
                    );
    }
}

void appview::keyPressEvent(QKeyEvent *event){
    if(event->text()=="\r"){
        Q_EMIT open();
    }

    if(event->key()==Qt::Key_Down){
        int row = this->currentIndex().row();
        if(row==2){
            this->clearSelection();
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
    if(this->currentIndex().row()==0){
        if(event->key()==Qt::Key_Up){
            this->clearSelection();
            Q_EMIT viewSwitchUp();
        }
    }

    QTreeView::keyPressEvent(event);
}
