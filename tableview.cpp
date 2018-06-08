#include "tableview.h"

#include <QStandardItemModel>

#include "itemdelegate.h"

TableView::TableView(QStringList items)
{
    resize(500,300);

    model = new QStandardItemModel(4,5);
    setModel(model);

    model->setHeaderData(0,Qt::Horizontal,QObject::tr("姓名"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("生日"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("职业"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("收入"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("日期"));

#if 1
    for(int col = 0; col < 5; ++col)
    {
        if(col == 0)
        {
            ReadOnlyDelegate *readOnly = new ReadOnlyDelegate;
            setItemDelegateForColumn(col,readOnly);
        }
        if(col == 1)
        {
            CheckBoxDelegate *checkBox = new CheckBoxDelegate;
            checkBox->setColumn(1);
            setItemDelegateForColumn(col,checkBox);
        }
        if(col == 2)
        {
            SpinBoxDelegate *spinBox = new SpinBoxDelegate;
            setItemDelegateForColumn(col,spinBox);
        }
        if(col == 3)
        {
            ComboDelegate *combo = new ComboDelegate;
            combo->setItems(items);
            setItemDelegateForColumn(col,combo);
        }
        if(col == 4){
            DateDelegate *date = new DateDelegate;
            setItemDelegateForColumn(col, date);
        }
        for(int row = 0; row < 4; ++row)
        {
            QModelIndex index = model->index(row, col, QModelIndex());
            if(col == 1)
                model->setData(index, 0);
            else
                model->setData(index, QString("%1%2").arg(row).arg(col));
        }
    }
#else
    QFile file("E:\Qt\MyDelegate\a.txt");
    QTextStream out(&file);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    while(out.atEnd()){
        QString data = out.readLine();
        QStringList dataList = data.split(",");
        int rows = model->rowCount();
        model->setRowCount(rows + 1);
        for(int col = 0; col < 4; ++col){
            QModelIndex index = model->index(rows, col, QModelIndex());
            model->setData(index,dataList.at(col));
        }
    }
#endif
}

QString TableView::GetItemData(int row, int col)
{
    QModelIndex index = model->index(row,col,QModelIndex());
    return model->data(index).toString();
}
