#include "dm_API.h"

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
}

bool dmExtent::operator==(dmExtent& other)
{
    this->topLeft  = other.topLeft;
    this->botRight = other.botRight;
}

bool const dmExtent::isWithin(const dmExtent& other)
{
    bool isInside = true;

    isInside &= (this.topLeft.north <= other.topLeft.north);
    isInside &= (this.topLeft.east  >= other.topLeft.east );
    isInside &= (this.botRight.north >= other.botRight.north);
    isInside &= (this.botRight.east  <= other.botRight.east );

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

    section.topLeft.north =
        (this.topLeft.north < other.topLeft.north) ?
        this.topLeft.north : other.topLeft.north;
    section.topLeft.east =
        (this.topLeft.east > other.topLeft.east) ?
        this.topLeft.east : other.topLeft.east;
    section.botRight.north =
        (this.botRight.north > other.botRight.north) ?
        this.botRight.north : other.botRight.north;
    section.botRight.east =
        (this.botRight.east < other.botRight.east) ?
        this.botRight.east : other.botRight.east;

    if (section.topLeft.north < section.botRight.north)
        return dmExtent();
    if (section.topLeft.east > section.botRight.east)
        return dmExtent();

    return section;
}

dmExtent const dmExtent::getMaxes(const dmExtent &other)
{
    dmExtent maxes;

    maxes.topLeft.north =
        (this.topLeft.north > other.topLeft.north) ?
        this.topLeft.north : other.topLeft.north;
    maxes.topLeft.east =
        (this.topLeft.east < other.topLeft.east) ?
        this.topLeft.east : other.topLeft.east;
    maxes.botRight.north =
        (this.botRight.north < other.botRight.north) ?
        this.botRight.north : other.botRight.north;
    maxes.botRight.east =
        (this.botRight.east > other.botRight.east) ?
        this.botRight.east : other.botRight.east;

    return maxes;
}

