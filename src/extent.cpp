#include "dm_API.h"

extent::extent()
    : topLeft(), botRight()
{}

extent::extent(coord topLeft, coord botRight)
    : topLeft(topLeft), botRight(botRight)
{}

extent::extent(double top, double left, double bot, double right)
    : topLeft(top, left), botRight(bot, right)
{}

extent& extent::operator=(const extent& other)
{
    this->topLeft  = other.topLeft;
    this->botRight = other.botRight;
}

bool extent::operator==(extent& other)
{
    this->topLeft  = other.topLeft;
    this->botRight = other.botRight;
}

bool const extent::isWithin(const extent& other)
{
    bool isInside = true;

    isInside &= (this.topLeft.north <= other.topLeft.north);
    isInside &= (this.topLeft.east  >= other.topLeft.east );
    isInside &= (this.botRight.north >= other.botRight.north);
    isInside &= (this.botRight.east  <= other.botRight.east );

    return isInside;
}

// If the section is empty (or zero-area), the extents are not overlapping.
bool const extent::overlaps(const extent& other)
{
    extent section = this->getSectionWith(other);

    if (section==extent())
        return false;
    else
        return true;
}

/* Currently returns default (0) value coordinates, if the section is empty,
 * or zero area. */
extent const extent::getSectionWith(const extent &other)
{
    extent section;

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
        return extent();
    if (section.topLeft.east > section.botRight.east)
        return extent();

    return section;
}

extent const extent::getMaxes(const extent &other)
{
    extent maxes;

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

