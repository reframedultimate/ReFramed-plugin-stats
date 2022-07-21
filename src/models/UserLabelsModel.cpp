#include "stats/models/UserLabelsModel.hpp"
#include <cstdio>
#include <memory>

// ----------------------------------------------------------------------------
static uint64_t hexStringToValue(const char* hex, int* error)
{
    uint64_t value = 0;

    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))
        hex += 2;

    for (; *hex; ++hex)
    {
        value <<= 4;
        if (*hex >= '0' && *hex <= '9')
            value |= *hex - '0';
        else if (*hex >= 'A' && *hex <= 'F')
            value |= *hex - 'A' + 10;
        else if (*hex >= 'a' && *hex <= 'f')
            value |= *hex - 'a' + 10;
        else
        {
            *error = 1;
            return 0;
        }
    }

    return value;
}

// ----------------------------------------------------------------------------
bool UserLabelsModel::loadMotionLabels(const char* fileName)
{
    FILE* fp = fopen(fileName, "r");
    if (fp == nullptr)
        return false;

    char line[128];
    while (fgets(line, sizeof(line), fp))
    {
        // Split string at comma
        char* delim = line;
        for (; *delim != ',' && *delim; ++delim) {}
        if (!*delim)
            continue;
        *delim = '\0';
        char* labelStr = delim + 1;

        // Remove newline and/or carriage return
        for (delim++; *delim != '\r' && *delim != '\n' && *delim; ++delim) {}
        *delim = '\0';

        int error = 0;
        rfcommon::FighterMotion motionValue = hexStringToValue(line, &error);
        if (motionValue == 0)
        {
            if (error)
                fprintf(stderr, "Failed to parse \"%s\" into hex value\n", line);
            else
                fprintf(stderr, "Invalid hex value \"%s\"\n", line);
            continue;
        }

        rfcommon::FighterMotion motion(motionValue);
        rfcommon::SmallString<31> label(labelStr);

        auto motionMapResult = motionLabels.motionMap.insertNew(motion, -1);
        if (motionMapResult == motionLabels.motionMap.end())
        {
            fprintf(stderr, "Duplicate motion value: %s\n", line);
            continue;
        }

        motionLabels.entries.emplace(motion, label);
        motionMapResult->value() = motionLabels.entries.count() - 1;
    }
    fclose(fp);

    fprintf(stderr, "Loaded %d motion labels\n", motionLabels.entries.count());

    return true;
}

// ----------------------------------------------------------------------------
const char* UserLabelsModel::motionToLabel(rfcommon::FighterMotion motion) const
{
    auto it = motionLabels.motionMap.find(motion);
    if (it == motionLabels.motionMap.end())
        return nullptr;
    const auto& s = motionLabels.entries[it->value()].label;
    return s.count() ? s.cStr() : nullptr;
}

UserLabelsModel::UserLabelsModel()
    : motionLabels()
{}
UserLabelsModel::~UserLabelsModel() {}


MotionLabels::MotionLabels() {}
MotionLabels::~MotionLabels() {}
