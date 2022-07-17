#pragma once

#include "stats/listeners/StatsListener.hpp"
#include <QWidget>

// Forward declare the class created by Qt designer
namespace Ui {
    class StatsView;
}

class StatsModel;

class StatsView : public QWidget
                , public StatsListener
{
    Q_OBJECT

public:
    explicit StatsView(StatsModel* model, QWidget* parent=nullptr);
    ~StatsView();

private:
    // We hold a weak reference to the model (StatsModel) so we can listen
    // to it
    StatsModel* model_;

    // This is an instance of the widget hierarchy we created in QtDesigner
    Ui::StatsView* ui_;
};
