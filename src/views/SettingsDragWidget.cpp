#include "stats/views/SettingsDragWidget.hpp"

#include <QDragEnterEvent>
#include <QMimeData>

// ----------------------------------------------------------------------------
SettingsDragWidget::SettingsDragWidget(QWidget* parent)
    : QFrame(parent)
{
}

// ----------------------------------------------------------------------------
void SettingsDragWidget::dragEnterEvent(QDragEnterEvent* e)
{
    if (e->mimeData()->hasFormat("application/x-stats-settings-item"))
    {
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
    else
    {
        e->ignore();
    }
}

// ----------------------------------------------------------------------------
void SettingsDragWidget::dragMoveEvent(QDragMoveEvent* e)
{

}

// ----------------------------------------------------------------------------
void SettingsDragWidget::dropEvent(QDropEvent* e)
{

}

// ----------------------------------------------------------------------------
void SettingsDragWidget::mousePressEvent(QMouseEvent* e)
{

}
