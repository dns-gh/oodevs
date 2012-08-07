// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PROPAGATION_FLOOD_MODEL_H
#define SWORD_PROPAGATION_FLOOD_MODEL_H

#include <boost/noncopyable.hpp>
#include <geometry/Types.h>

namespace sword
{
namespace wrapper
{
    class Node;
    class Event;
}
namespace propagation
{
// =============================================================================
/** @class  FloodModel
    @brief  Flood model
*/
// Created: LGY 2012-08-01
// =============================================================================
class FloodModel : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodModel();
    virtual ~FloodModel();
    //@}

    //! @name Operations
    //@{
    void Generate( const geometry::Point2f& center, wrapper::Event& event, int depth, int refDist ) const;
    //@}

private:
    //! @name Types
    //@{
    struct sCell
    {
        sCell()
            : visited_( false )
            , polIndex_( 0 )
            , deep_( false )
        {}
        bool visited_;
        int polIndex_;
        bool deep_;
    };
    //@}

private:
    //! @name Helpers
    //@{
    void Propagate( int floodElevation, unsigned short halfWidth, const geometry::Point2f& center, sCell** ppCells, int refDist ) const;
    bool FindFirstUnmarkedCell( int& xRet, int& yRet, unsigned short halfWidth, sCell** ppCells ) const;
    bool FindFirstMarkedCell( int& xRet, int& yRet, int index, unsigned short halfWidth, sCell** ppCells ) const;
    int MarkAdjacentCells( int x, int y, bool deep, int nPolygonIndex, unsigned short halfWidth, sCell** ppCells ) const;
    void MarkCells( int xStart, int yStart, int nPolygonIndex, unsigned short halfWidth, sCell** ppCells ) const;
    void CreatePolygon( int nPolygonIndex, const geometry::Point2f& center, unsigned short halfWidth, sCell** ppCells, wrapper::Node& deepAreas, wrapper::Node& lowAreas ) const;
    int FindUnmarkedOnNextLine( int xStart, int xEnd, int y, bool deep, unsigned short halfWidth, sCell** ppCells ) const;
    int FindFirstMarkedOnLine( int y, int index, unsigned short halfWidth, sCell** ppCells ) const;
    int FindLastMarkedOnLine( int y, int index, unsigned short halfWidth, sCell** ppCells ) const;
    //@}

private:
    //! @name Member data
    //@{
    static const int cellWidth_;
    //@}
};

}
}

#endif // SWORD_PROPAGATION_FLOOD_MODEL_H
