#include "stats/views/StatsView.hpp"
#include "stats/models/StatsCalculator.hpp"
#include "stats/models/SettingsModel.hpp"
#include "stats/util/StatsFormatter.hpp"

#include <QGridLayout>
#include <QLabel>

// ----------------------------------------------------------------------------
static void clearLayout(QLayout* layout)
{
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        if (item->layout() != nullptr)
            item->layout()->deleteLater();
        if (item->widget() != nullptr)
            item->widget()->deleteLater();
    }
}

// ----------------------------------------------------------------------------
StatsView::StatsView(StatsCalculator* stats, SettingsModel* settings, QWidget* parent)
    : QWidget(parent)
    , stats_(stats)
    , settings_(settings)
    , layout_(new QGridLayout)
{
    setLayout(layout_);
    recreateLayout();
    updateStatsLabels();

    // Listen to any changes to the model
    stats_->dispatcher.addListener(this);
    settings_->dispatcher.addListener(this);
}

// ----------------------------------------------------------------------------
StatsView::~StatsView()
{
    // Remove things in reverse order
    settings_->dispatcher.removeListener(this);
    stats_->dispatcher.removeListener(this);
}

// ----------------------------------------------------------------------------
void StatsView::updateStatsLabels()
{
    StatsFormatter fmt(stats_);
    for (int i = 0; i != settings_->numStatsEnabled(); ++i)
    {
        StatType statType = settings_->statAtIndex(i);
        if (settings_->statEnabled(statType) == false)
            continue;

        QLabel* p1Label = qobject_cast<QLabel*>(layout_->itemAtPosition(i + 2, 0)->widget());
        QLabel* p2Label = qobject_cast<QLabel*>(layout_->itemAtPosition(i + 2, 4)->widget());

        p1Label->setText(fmt.playerStatAsString(0, statType));
        p2Label->setText(fmt.playerStatAsString(1, statType));
    }
}

// ----------------------------------------------------------------------------
void StatsView::recreateLayout()
{
    clearLayout(layout_);

    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(14);

    QFont statsFont;
    statsFont.setPointSize(12);

    QLabel* p1Label = new QLabel("Player 1");
    p1Label->setFont(titleFont);
    layout_->addWidget(p1Label, 0, 0, Qt::AlignRight);

    QLabel* p2Label = new QLabel("Player 2");
    p2Label->setFont(titleFont);
    layout_->addWidget(p2Label, 0, 4, Qt::AlignLeft);

    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout_->addWidget(line, 1, 0, 1, 5);

    for (int i = 0; i != settings_->numStatsEnabled(); ++i)
    {
        StatType statType = settings_->statAtIndex(i);
        if (settings_->statEnabled(statType) == false)
            continue;

        QLabel* label = new QLabel(statTypeToString(statType));
        label->setFont(titleFont);

        QLabel* p1Stat = new QLabel;
        p1Stat->setFont(statsFont);

        QLabel* p2Stat = new QLabel;
        p2Stat->setFont(statsFont);

        layout_->addWidget(p1Stat, i + 2, 0, Qt::AlignRight);
        layout_->addWidget(label, i + 2, 2, Qt::AlignCenter);
        layout_->addWidget(p2Stat, i + 2, 4, Qt::AlignLeft);
    }
}

// ----------------------------------------------------------------------------
void StatsView::onStatsUpdated()
{
    updateStatsLabels();
}

// ----------------------------------------------------------------------------
void StatsView::onSettingsStatTypesChanged()
{
    recreateLayout();
    updateStatsLabels();
}
