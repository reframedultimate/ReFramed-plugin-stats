#pragma once

#include "rfcommon/ListenerDispatcher.hpp"

namespace rfcommon {
    class Session;
}

class StatsListener;

class StatsModel
{
public:
    /*!
     * \brief Calculates all of the statistics from the given session and
     * stores the results into the model. You can use the getters below to
     * get the results after calling this method.
     */
    void calculateStatistics(rfcommon::Session* session);

    rfcommon::ListenerDispatcher<StatsListener> dispatcher;
};
