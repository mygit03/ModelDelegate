#include "widget.h"
#include "ui_widget.h"

#include <QStandardItemModel>
#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "tableview.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(500,300);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    m_pTabWidget = new QTabWidget(this);
    hlayout->addWidget(m_pTabWidget);

    QPushButton *btn = new QPushButton(tr("获取数据"),this);
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(onBtn_clicked()));
    hlayout->addWidget(btn);

    QStringList items;
    items.append("农民");
    items.append("工人");
    items.append("教师");
    items.append("音乐1");
    TableView *table1 = new TableView(items);
    m_pTabWidget->addTab(table1,tr("tab1"));

    items.clear();
    items.append("农民1");
    items.append("工人1");
    items.append("教师1");
    items.append("音乐2");
    TableView *table2 = new TableView(items);
    m_pTabWidget->addTab(table2,tr("tab2"));

    setLayout(hlayout);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onBtn_clicked()
{
    TableView *table = static_cast<TableView *>(m_pTabWidget->currentWidget());
    for(int row = 0; row < 4; ++row)
    {
        QString data = table->GetItemData(0, row);
        qDebug() << "data:" << data;
    }
}
