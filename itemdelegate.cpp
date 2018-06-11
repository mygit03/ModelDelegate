#include "itemdelegate.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QDebug>

/****************************************************
 * ComboDelegate
 ****************************************************/
ComboDelegate::ComboDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void ComboDelegate::setItems(QStringList items)
{
    m_sItemList = items;
}

QWidget *ComboDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->addItems(m_sItemList);
    editor->installEventFilter(const_cast<ComboDelegate*>(this));
    return editor;
}

void ComboDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString str =index.model()->data(index).toString();

    QComboBox *box = static_cast<QComboBox*>(editor);
    int i=box->findText(str);
    box->setCurrentIndex(i);
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);
    QString str = box->currentText();
    model->setData(index,str);
}

void ComboDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

/****************************************************
 * SpinBoxDelegate
 ****************************************************/
SpinBoxDelegate::SpinBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    //初始化最大最小值
    minValue = 1.00;
    maxValue = 100.00;
}

void SpinBoxDelegate::setRange(double min, double max)
{
    minValue = min;
    maxValue = max;
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox *editor =  new QDoubleSpinBox(parent);
    editor->setMinimum(minValue);
    editor->setMaximum(maxValue);
    editor->setSingleStep(0.01);
    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();
    QDoubleSpinBox *spinBox =  static_cast <QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox =  static_cast <QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

/****************************************************
 * CheckBoxDelegate
 ****************************************************/
static QRect CheckBoxRect(const QStyleOptionViewItem &viewItemStyleOptions)/*const*/
{
    //绘制按钮所需要的参数
    QStyleOptionButton checkBoxStyleOption;
    //按照给定的风格参数 返回元素子区域
    QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &checkBoxStyleOption);
    //返回QCheckBox坐标
    QPoint checkBoxPoint(viewItemStyleOptions.rect.x() + viewItemStyleOptions.rect.width() / 2 - checkBoxRect.width() / 2,
                         viewItemStyleOptions.rect.y() + viewItemStyleOptions.rect.height() / 2 - checkBoxRect.height() / 2);
    //返回QCheckBox几何形状
    return QRect(checkBoxPoint, checkBoxRect.size());
}

CheckBoxDelegate::CheckBoxDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void CheckBoxDelegate::setColumn(int col)
{
    column = col;
}

// 绘制复选框
void CheckBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,const QModelIndex& index)const
{
    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();

    if(index.column() == column){
        QStyleOptionButton checkBoxStyleOption;
        checkBoxStyleOption.state |= QStyle::State_Enabled;
        checkBoxStyleOption.state |= checked? QStyle::State_On : QStyle::State_Off;
        checkBoxStyleOption.rect = CheckBoxRect(option);

        QApplication::style()->drawControl(QStyle::CE_CheckBox,&checkBoxStyleOption,painter);
    }else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}

// 响应鼠标事件，更新数据
bool CheckBoxDelegate::editorEvent(QEvent *event,
                                QAbstractItemModel *model,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) {
    if(index.column() == column){
    if((event->type() == QEvent::MouseButtonRelease) ||
            (event->type() == QEvent::MouseButtonDblClick)){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() != Qt::LeftButton ||
                !CheckBoxRect(option).contains(mouseEvent->pos())){
            return true;
        }
        if(event->type() == QEvent::MouseButtonDblClick){
            return true;
        }
    }else if(event->type() == QEvent::KeyPress){
        if(static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
                static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select){
            return false;
        }
    }else{
        return false;
    }

    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
    int data = checked ? 0 : 1;     //互逆
    return model->setData(index, data, Qt::EditRole);
    }else{
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
}

/****************************************************
 * DateDelegate
 ****************************************************/
DateDelegate::DateDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *DateDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const
{
    QDateEdit *editor = new QDateEdit(parent);
    editor->setDisplayFormat("yyyy-MM-dd");
    editor->setCalendarPopup(true);
    return editor;
}

void DateDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString str =index.model()->data(index).toString();

    QDateEdit *pDate = static_cast<QDateEdit*>(editor);
    pDate->setDate(QDate::fromString(str,"yyyy-MM-dd"));
}

void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit *pDate = static_cast<QDateEdit*>(editor);
    QString str = pDate->date().toString("yyyy-MM-dd");
    model->setData(index,str);
}

void DateDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

/****************************************************
 * TimeDelegate
 ****************************************************/
TimeDelegate::TimeDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *TimeDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const
{
    QTimeEdit *editor = new QTimeEdit(parent);
    editor->setDisplayFormat("hh:mm:ss");
    editor->setCalendarPopup(true);
    return editor;
}

void TimeDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString str =index.model()->data(index).toString();

    QTimeEdit *pTime = static_cast<QTimeEdit*>(editor);
    pTime->setTime(QTime::fromString(str,"hh:mm:ss"));
}

void TimeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTimeEdit *pTime = static_cast<QTimeEdit*>(editor);
    QString str = pTime->time().toString("hh:mm:ss");
    model->setData(index,str);
}

void TimeDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

/****************************************************
 * DoubleLineEditDelegate
 ****************************************************/
DoubleLineEditDelegate::DoubleLineEditDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    min = 0;
    max = 99;
}

void DoubleLineEditDelegate::setRange(double bottom, double top, int decimals)
{
    min = bottom;
    max = top;
    dec = decimals;
}

QWidget *DoubleLineEditDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setValidator(new QDoubleValidator(min, max, dec));
    return editor;
}

void DoubleLineEditDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString str =index.model()->data(index).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(str);
}

void DoubleLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString str = lineEdit->text();
    model->setData(index,str);
}

void DoubleLineEditDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
