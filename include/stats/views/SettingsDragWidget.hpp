#pragma once

#include <QFrame>

class SettingsDragWidget : public QFrame
{
public:
    explicit SettingsDragWidget(QWidget* parent=nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dragMoveEvent(QDragMoveEvent* e) override;
    void dropEvent(QDropEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
};
