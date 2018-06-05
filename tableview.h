#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>

class QStandardItemModel;

class TableView : public QTableView
{
    Q_OBJECT

public:
    TableView(QStringList items);

    QString GetItemData(int row, int col);

private:
    QStandardItemModel *model;
};

#endif // TABLEVIEW_H
