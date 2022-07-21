#pragma once

#include "stats/models/UserLabelCategory.hpp"
#include "rfcommon/Types.hpp"
#include "rfcommon/Vector.hpp"
#include "rfcommon/String.hpp"
#include "rfcommon/HashMap.hpp"

class MotionLabels
{
public:
    MotionLabels();
    ~MotionLabels();

    struct Entry
    {
        Entry(rfcommon::FighterMotion motion, const rfcommon::SmallString<31>& label) : motion(motion), label(label) {}

        rfcommon::FighterMotion motion;
        rfcommon::SmallString<31> label;
    };

    struct FighterMotionHasher
    {
        typedef uint32_t HashType;
        HashType operator()(rfcommon::FighterMotion value) const {
            return rfcommon::hash32_jenkins_oaat(&value, sizeof(rfcommon::FighterMotion));
        }
    };

    rfcommon::Vector<Entry> entries;
    rfcommon::HashMap<rfcommon::FighterMotion, int, FighterMotionHasher> motionMap;
};

class UserLabelsModel
{
public:
    UserLabelsModel();
    ~UserLabelsModel();

    bool loadMotionLabels(const char* fileName);

    const char* motionToLabel(rfcommon::FighterMotion motion) const;

private:
    MotionLabels motionLabels;
};
