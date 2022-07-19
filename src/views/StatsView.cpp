#include "stats/views/StatsView.hpp"
#include "stats/models/StatsModel.hpp"
#include "stats/models/SettingsModel.hpp"

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
StatsView::StatsView(StatsModel* stats, SettingsModel* settings, QWidget* parent)
    : QWidget(parent)
    , stats_(stats)
    , settings_(settings)
    , layout_(new QGridLayout)
{
    setLayout(layout_);
    StatsView::onSettingsChanged();
    StatsView::onStatsUpdated();

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
void StatsView::onStatsUpdated()
{
    for (int i = 0; i != STAT_COUNT; ++i)
    {
        StatisticType statType = static_cast<StatisticType>(i);
        if (settings_->statEnabled(statType) == false)
            continue;

        int statIdx = settings_->statPosition(statType);
        QLabel* p1Label = qobject_cast<QLabel*>(layout_->itemAtPosition(statIdx + 2, 0)->widget());
        QLabel* p2Label = qobject_cast<QLabel*>(layout_->itemAtPosition(statIdx + 2, 4)->widget());

        switch (statType)
        {
        case STAT_AVERAGE_DAMAGE_PER_OPENING:
            p1Label->setText(QString::number(stats_->avgDamagePerOpening(0), 'g', 1) + "%");
            p2Label->setText(QString::number(stats_->avgDamagePerOpening(1), 'g', 1) + "%");
            break;
        case STAT_AVERAGE_KILL_PERCENT:
            p1Label->setText(QString::number(stats_->avgKillPercent(0), 'g', 1) + "%");
            p2Label->setText(QString::number(stats_->avgKillPercent(1), 'g', 1) + "%");
            break;
        case STAT_EARLIEST_KILL:
            p1Label->setText(QString::number(stats_->earliestKillPercent(0), 'g', 1) + "%");
            p2Label->setText(QString::number(stats_->earliestKillPercent(1), 'g', 1) + "%");
            break;
        case STAT_LATEST_DEATH:
            p1Label->setText(QString::number(stats_->latestDeathPercent(0), 'g', 1) + "%");
            p2Label->setText(QString::number(stats_->latestDeathPercent(1), 'g', 1) + "%");
            break;
        case STAT_NEUTRAL_WINS:
            p1Label->setText(QString::number(stats_->numNeutralWins(0)));
            p2Label->setText(QString::number(stats_->numNeutralWins(1)));
            break;
        case STAT_NEUTRAL_WIN_PERCENT:
            p1Label->setText(QString::number(stats_->neutralWinPercent(0), 'g', 1) + "%");
            p2Label->setText(QString::number(stats_->neutralWinPercent(1), 'g', 1) + "%");
            break;
        case STAT_OPENINGS_PER_KILL:
            p1Label->setText(QString::number(stats_->numOpeningsPerKill(0)));
            p2Label->setText(QString::number(stats_->numOpeningsPerKill(1)));
            break;
        case STAT_STOCKS_TAKEN:
            p1Label->setText(QString::number(stats_->numStocksTaken(0)));
            p2Label->setText(QString::number(stats_->numStocksTaken(1)));
            break;
        case STAT_STAGE_CONTROL_PERCENT:
            p1Label->setText(QString::number(stats_->stageControlPercent(0), 'g', 1) + "%");
            p2Label->setText(QString::number(stats_->stageControlPercent(1), 'g', 1) + "%");
            break;
        case STAT_TOTAL_DAMAGE_DEALT:
            p1Label->setText(QString::number(stats_->totalDamageDealt(0), 'g', 1) + "%");
            p2Label->setText(QString::number(stats_->totalDamageDealt(1), 'g', 1) + "%");
            break;

        default:
            break;
        }
    }
}

// ----------------------------------------------------------------------------
void StatsView::onSettingsChanged()
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

    for (int i = 0; i != STAT_COUNT; ++i)
    {
        StatisticType statType = static_cast<StatisticType>(i);
        if (settings_->statEnabled(statType) == false)
            continue;

        QLabel* label = new QLabel(statisticTypeToString(statType));
        label->setFont(titleFont);

        QLabel* p1Stat = new QLabel;
        p1Stat->setFont(statsFont);

        QLabel* p2Stat = new QLabel;
        p2Stat->setFont(statsFont);

        int statIdx = settings_->statPosition(statType);
        layout_->addWidget(p1Stat, statIdx + 2, 0, Qt::AlignRight);
        layout_->addWidget(label, statIdx + 2, 2, Qt::AlignCenter);
        layout_->addWidget(p2Stat, statIdx + 2, 4, Qt::AlignLeft);
    }
}
