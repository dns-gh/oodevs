// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __tools_CONTOURCOMPUTER_H_
#define __tools_CONTOURCOMPUTER_H_

#include <geometry/Types.h>
#include <boost/function.hpp>

namespace tools
{

typedef std::vector< geometry::Point2f > T_PointVector;

// Compute contour lines from an ascii grid.
// width, height : maximum indices of the grid
// linesHeight : differences between contour lines
// cellSize : cell size
// data : functor to retrieve the data
// progress : functor to display progress
// valid : functor to check if data is still valid
// loop : functor which will receive the contour lines. Parameters are line
//   content, index, boolean indicating that the contour is closed
// checkStop : functor to check if computation should stop
// returns true if completed, false otherwise
bool ComputeContour( const unsigned int width, const unsigned int height,
        const int linesHeight, const float cellSize,
        boost::function< short(int, int) > data,
        boost::function< void(short) > progress,
        boost::function< bool() > valid,
        boost::function< void( boost::shared_ptr< T_PointVector >, int, bool )> loop,
        boost::function< bool() > checkStop, const int nc );

} // namespace tools

#endif // __tools_CONTOURCOMPUTER_H_
