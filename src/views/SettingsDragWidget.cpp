#include "stats/views/SettingsDragWidget.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>

#include <QDebug>

// ----------------------------------------------------------------------------
SettingsDragWidget::SettingsDragWidget(QWidget* parent)
    : QFrame(parent)
{
    setAcceptDrops(true);

    setLayout(new QVBoxLayout);
}

// ----------------------------------------------------------------------------
void SettingsDragWidget::dragEnterEvent(QDragEnterEvent* e)
{
    if (!e->mimeData()->hasFormat("application/x-stats-settings-item"))
    {
        e->ignore();
        return;
    }

    if (e->source() == this)
    {
        e->setDropAction(Qt::MoveAction);
        e->accept();
    }
    else
    {
        e->acceptProposedAction();
    }
}

// ----------------------------------------------------------------------------
void SettingsDragWidget::dragMoveEvent(QDragMoveEvent* e)
{
    if (!e->mimeData()->hasFormat("application/x-stats-settings-item"))
    {
        e->ignore();
        return;
    }

    if (e->source() == this)
    {
        e->setDropAction(Qt::MoveAction);
        e->accept();
    }
    else
    {
        e->acceptProposedAction();
    }
}

// ----------------------------------------------------------------------------
void SettingsDragWidget::dropEvent(QDropEvent* e)
{
    if (!e->mimeData()->hasFormat("application/x-stats-settings-item"))
    {
        e->ignore();
        return;
    }

    QByteArray itemData = e->mimeData()->data("application/x-stats-settings-item");
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QString text;
    dataStream >> text;

    QLabel* label = new QLabel;
    label->setText(text);
    
    int insertIndex = 0;
    while (insertIndex != layout()->count())
    {
        QLayoutItem* item = layout()->itemAt(insertIndex);
        QRect g = item->geometry();
        int dropY = e->pos().y();
        int itemHeight = g.height();
        int itemY = g.y() + itemHeight / 2;

        if (dropY < itemY)
            break;

        insertIndex++;
    }

    static_cast<QVBoxLayout*>(layout())->insertWidget(insertIndex, label);

    if (e->source() == this)
    {
        e->setDropAction(Qt::MoveAction);
        e->accept();
    }
    else
    {
        e->acceptProposedAction();
    }
}

// ----------------------------------------------------------------------------
void SettingsDragWidget::mousePressEvent(QMouseEvent* e)
{
    QLabel* item = qobject_cast<QLabel*>(childAt(e->pos()));
    if (item == nullptr)
        return;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << item->text();

    QMimeData* mimeData = new QMimeData;
    mimeData->setData("application/x-stats-settings-item", itemData);

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(e->pos() - item->pos());

    QString originalText = item->text();
    item->setText("...");

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
        delete item;
    else
        item->setText(originalText);

}
