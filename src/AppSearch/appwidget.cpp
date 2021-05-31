#include "appwidget.h"

AppWidget::AppWidget()
{
    initSettingsearchUI();
    installEventFilter(this);
}

void AppWidget::initSettingsearchUI()
{

    m_listLayout = new QVBoxLayout(this);
    m_Headlabel = new HeadLabel(this);
    m_Headlabel->setText(tr("App"));

    settingView = new appview;
    m_settingmodel = new appModel;
    settingView->setModel(m_settingmodel);

    m_Button=new MoreButton(this);
    m_Button->setText("在应用商店中搜索更多");

    m_listLayout->addWidget(m_Headlabel);
    m_listLayout->addWidget(settingView);
    m_listLayout->addWidget(m_Button);
    this->setLayout(m_listLayout);
    this->setVisible(false);

    connect(m_Button,&MoreButton::clicked,this,[=](){
        QProcess *process =new QProcess(this);
        process->startDetached("ubuntu-kylin-software-center");
    });

    connect(m_Button,&MoreButton::open,this,[=](){
        QProcess *process =new QProcess(this);
        process->startDetached("ubuntu-kylin-software-center");
    });

    connect(m_Button,&MoreButton::switchUpModule,this,[=](){
        settingView->setCurrentIndex(m_settingmodel->index(2,0,m_settingmodel->index(0,0,m_settingmodel->index(0))));
    });

    connect(m_Button,&MoreButton::switchDownModule,this,[=](){
        Q_EMIT viewSwitchDown();
    });

    //监听点击事件，打开对应的设置选项

    connect(settingView,&appview::open,this,[=](){
        m_settingmodel->run(settingView->currentIndex().row());
    });

    connect(settingView,&QTreeView::clicked,this,[=](){
        m_settingmodel->run(settingView->currentIndex().row());
    });

    connect(settingView,&appview::viewSwitchUp,this,[=](){
        Q_EMIT viewSwitchUp();
    });

    connect(settingView,&appview::viewSwitchDown,this,[=](){
       m_Button->setFocus();
       settingView->clearSelection();
    });

    connect(settingView,&appview::viewNoMoreSwitchDown,this,[=](){
       settingView->clearSelection();
       Q_EMIT viewSwitchDown();
    });

    //监听搜索出的设置数量，做界面更改
    connect(m_settingmodel,&appModel::requestUpdateSignal,this,&AppWidget::recvSettingSearchResult);

    //监听输入框的改变，刷新界面
    QDBusConnection::sessionBus().connect(QString(), QString("/lineEdit/textChanged"), "org.ukui.search.inputbox", "InputBoxTextChanged", this, SLOT(settingTextRefresh(QString)));

}

void AppWidget::settingTextRefresh(QString mSearchText)
{
    m_settingmodel->matchstart(mSearchText);
}

void AppWidget::recvSettingSearchResult(int row)
{
    if(row>2){
        this->setFixedHeight(m_Headlabel->height()+(row+1.7)*46);
        m_Button->show();
    }else{
        this->setFixedHeight(m_Headlabel->height()+(row+1)*46);
        m_Button->hide();
    }
    if(row<=0){
        this->setVisible(false);
    } else {
        this->setVisible(true);
    }
}


void AppWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    QRect rect = this->rect();
    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    p.setBrush(qApp->palette().color(QPalette::Base));
//    p.setBrush(QBrush(QColor(255,255,255)));
    p.setOpacity(1);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(rect,12,12);
    QWidget::paintEvent(e);
}

bool AppWidget::eventFilter(QObject *watched, QEvent *event){
    if(watched==this){
        if(event->type()==QEvent::MouseButtonRelease){
            return true;
        }
    }
    return false;
}

void AppWidget::selectLastRow(){
    if(m_Button->isVisible()){
        m_Button->setFocus();
    }else{
        settingView->setFocus();
        settingView->setCurrentIndex(lastVisibleItem(settingView));
    }
}


void AppWidget::fristSelect(){
    QTimer::singleShot(100,[this] {
        settingView->setFocus();
        settingView->setCurrentIndex(m_settingmodel->index(0));});
}

QModelIndex AppWidget::lastVisibleItem(appview *view, const QModelIndex &index )
{
    QAbstractItemModel *model = view->model();
    int rowCount = model->rowCount(index);
    if (rowCount> 0) {
        //Find the last item in this level of hierarchy.
        QModelIndex lastIndex = model->index(rowCount - 1, 0, index);
        if (model->hasChildren(lastIndex) && view->isExpanded(lastIndex)) {
            //There is even deeper hierarchy. Drill down with recursion.
            return lastVisibleItem(view, lastIndex);
        } else {
            //Test the last item in the tree.
            return lastIndex;
        }
    } else {
        return QModelIndex();
    }
}
