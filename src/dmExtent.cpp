#include "dmExtent.h"
#include <algorithm>

dmExtent::dmExtent()
    : topLeft(), botRight()
{}

dmExtent::dmExtent(coord topLeft, coord botRight)
    : topLeft(topLeft), botRight(botRight)
{}

dmExtent::dmExtent(double top, double left, double bot, double right)
    : topLeft(top, left), botRight(bot, right)
{}

dmExtent& dmExtent::operator=(const dmExtent& other)
{
    this->topLeft  = other.topLeft;
    this->botRight = other.botRight;

    return *this;
}

bool dmExtent::operator==(const dmExtent& other)
{
    if (this->topLeft == other.topLeft &&
        this->botRight == other.botRight)
        return true;
    else
        return false;
}

bool const dmExtent::isWithin(const dmExtent& other)
{
    bool isInside = true;

    isInside &= (this->topLeft.north <= other.topLeft.north);
    isInside &= (this->topLeft.east  >= other.topLeft.east );
    isInside &= (this->botRight.north >= other.botRight.north);
    isInside &= (this->botRight.east  <= other.botRight.east );

    return isInside;
}

// If the section is empty (or zero-area), the extents are not overlapping.
bool const dmExtent::overlaps(const dmExtent& other)
{
    dmExtent section = this->getSectionWith(other);

    if (section== dmExtent())
        return false;
    else
        return true;
}

/* Currently returns default (0) value coordinates, if the section is empty,
 * or zero area. */
dmExtent const dmExtent::getSectionWith(const dmExtent &other)
{
    dmExtent section;

    section.topLeft.north = std::min(this->topLeft.north, other.topLeft.north);
    section.topLeft.east = std::max(this->topLeft.east, other.topLeft.east);
    section.botRight.north = std::max(this->botRight.north, other.botRight.north);
    section.botRight.east = std::min(this->botRight.east, other.botRight.east);

    if (section.topLeft.north < section.botRight.north)
        return dmExtent();
    if (section.topLeft.east > section.botRight.east)
        return dmExtent();

    return section;
}

dmExtent const dmExtent::getMaxes(const dmExtent &other)
{
    dmExtent maxes;

    maxes.topLeft.north = std::max(this->topLeft.north, other.topLeft.north);
    maxes.topLeft.east = std::min(this->topLeft.east, other.topLeft.east);
    maxes.botRight.north = std::min(this->botRight.north, other.botRight.north);
    maxes.botRight.east = std::max(this->botRight.east, other.botRight.east);

    return maxes;
}
