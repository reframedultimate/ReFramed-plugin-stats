#include "ui_StatsView.h"
#include "stats/views/StatsView.hpp"
#include "stats/models/StatsModel.hpp"
#include <QLabel>

// ----------------------------------------------------------------------------
StatsView::StatsView(StatsModel* model, QWidget* parent)
    : QWidget(parent)
    , model_(model)
    , ui_(new Ui::StatsView)  // Instantiate UI created in QtDesigner
{
    // Set up UI created in QtDesigner
    ui_->setupUi(this);

    // Listen to any changes to the model
    model_->dispatcher.addListener(this);
}

// ----------------------------------------------------------------------------
StatsView::~StatsView()
{
    // Remove things in reverse order
    model_->dispatcher.removeListener(this);
    delete ui_;
}
