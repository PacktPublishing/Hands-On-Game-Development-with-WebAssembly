#include "game.hpp"

Range::Range() {
    min = 0;
    max = 0;
}

Range::Range( float min_val, float max_val ) {
    min = min_val;
    max = max_val;
}

bool Range::Overlap( Range &other ) {
    return other.min <= max && min <= other.max;
}

void Range::operator+=(const Range& range) {
    if( max < range.max ) {
        max = range.max;
    }

    if( min > range.min ) {
        min = range.min;
    }
}

Range Range::operator+(const Range& range) {
    Range combined;
    combined.min = min;
    combined.max = max;
    if (range.min < min) {
        combined.min = range.min;
    }

    if (range.max > max) {
        combined.max = range.max;
    }
    return combined;
}

void Range::Sort() {
    if (min > max) {
        float temp = min;
        min = max;
        max = temp;
    }
}

void Range::Extend( float ex ) {
    if (ex > max) {
        max = ex;
    }
    else if (ex < min) {
        min = ex;
    }
}

float Range::Clamp( float value ) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

Range Range::operator=(const Range& range) {
    min = range.min;
    max = range.max;
    return *this;
}


/*
class Range {
    public:
        float min;
        float max;

        Range();
        Range( float min_val, float max_val );
        bool Overlap( Range &other );
        void operator+=(const Range& range);
        Range operator+(const Range& range);
        Sort();
        void Extend( float ex );
        void Clamp( float clap_val );

        Range operator=(const Range& range);

}
*/