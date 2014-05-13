// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ApproximativeMap.h"
#include <cmath>

using namespace kernel;

namespace
{
    static const int indexOf[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//      !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /
    0, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 38, 38, 38, 38,
//  0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?
    1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 38, 38, 37, 37, 37, 37,
//  @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O
   37, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
//  P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _
   26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 37, 37, 38,
//  `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o
   37, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
//  p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~
   26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 37, 37,  0,

    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };

    static const int bitCount[256] = {
    0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
    1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
    1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
    2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
    2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
    1,  2,  2,  3,  2,  3,  3,  4,  2,  3,  3,  4,  3,  4,  4,  5,
    2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
    2,  3,  3,  4,  3,  4,  4,  5,  3,  4,  4,  5,  4,  5,  5,  6,
    3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
    3,  4,  4,  5,  4,  5,  5,  6,  4,  5,  5,  6,  5,  6,  6,  7,
    4,  5,  5,  6,  5,  6,  6,  7,  5,  6,  6,  7,  6,  7,  7,  8
    };

    struct CoMatrix
    {
        /*
        The matrix has 40 * 40 = 1600 entries.  This requires 200 bytes, or 50
        words.  Some operations are performed on words for more efficiency.
        */
        union
        {
            unsigned char b[200];
            unsigned long w[50];
        };

        CoMatrix()
        {
            ::memset( b, 0, 200 );
        }
        CoMatrix( const char* text )
        {
            char c = '\0', d;
            ::memset( b, 0, 200 );

            while ( (d = *text) != '\0' )
            {
                setCoocc( c, d );
                if( (c = *++text) != '\0' )
                {
                    setCoocc( d, c );
                    text++;
                }
            }
        }

        void setCoocc( char c, char d )
        {
            int k = indexOf[(unsigned char) c] + 40 * indexOf[(unsigned char) d];
            b[k >> 3] |= k & 0x7;
        }

        int worth() const
        {
            int w = 0;
            for ( int i = 0; i < 200; i++ )
                w += bitCount[ b[i] ];
            return w;
        }
    };

    static CoMatrix reunion( const CoMatrix& m, const CoMatrix& n )
    {
        CoMatrix p;
        for ( int i = 0; i < 50; i++ )
            p.w[i] = m.w[i] | n.w[i];
        return p;
    }

    static CoMatrix intersection( const CoMatrix& m, const CoMatrix& n )
    {
        CoMatrix p;
        for ( int i = 0; i < 50; i++ )
            p.w[i] = m.w[i] & n.w[i];
        return p;
    }

}

// -----------------------------------------------------------------------------
// Name: internal::Find
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
int ::kernel::internal::Find( const std::string& text, const std::vector< std::string >& candidates, int threshold )
{
    std::vector< int > scores;
    CoMatrix cmTarget( text.c_str() );
    const int targetLen = (int)text.size();

    int bestScore = -1;
    int bestIndex = -1;
    for( std::vector< std::string >::const_iterator it = candidates.begin(); it != candidates.end(); ++it )
    {
        const std::string& s = *it;
        CoMatrix cm( s.c_str() );
        const int delta = std::abs( int( s.length() ) - targetLen );
        const int score = ( (intersection( cm, cmTarget ).worth() + 1 ) << 10 )
                        / ( reunion( cm, cmTarget ).worth() + (delta << 1) + 1 );
        if( score > bestScore && score > threshold )
        {
            bestIndex = (int)(it - candidates.begin());
            bestScore = score;
        }
    }
    return bestIndex;
}
