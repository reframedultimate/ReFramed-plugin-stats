#pragma once

#include "stats/models/StatisticType.hpp"
#include <QFrame>

class SettingsDragWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SettingsDragWidget(QWidget* parent=nullptr);

    void addStat(StatisticType type);
    void removeStat(StatisticType type);

signals:
    void statAdded(StatisticType type);
    void statRemoved(StatisticType type);

protected:
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dragMoveEvent(QDragMoveEvent* e) override;
    void dropEvent(QDropEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
};
