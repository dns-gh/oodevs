// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodModel_h_
#define __FloodModel_h_

#include "FloodModel_ABC.h"
#include <geometry/types.h>

namespace propagation
{
    class ElevationGetter_ABC;

// =============================================================================
/** @class  FloodModel
    @brief  FloodModel
*/
// Created: JSR 2010-12-08
// =============================================================================
class FloodModel : public FloodModel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FloodModel( const ElevationGetter_ABC& getter );
    virtual ~FloodModel();
    //@}

    //! @name Operations
    //@{
    virtual void GenerateFlood( const geometry::Point2d& center, T_Polygons& deepAreas, T_Polygons& lowAreas, int depth, int refDist ) const;
    virtual void GenerateFlood( const geometry::Point2f& center, T_Polygons& deepAreas, T_Polygons& lowAreas, int depth, int refDist ) const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
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
    void CreatePolygon( int nPolygonIndex, const geometry::Point2f& center, unsigned short halfWidth, sCell** ppCells, T_Polygons& deepAreas, T_Polygons& lowAreas ) const;
    int FindUnmarkedOnNextLine( int xStart, int xEnd, int y, bool deep, unsigned short halfWidth, sCell** ppCells ) const;
    int FindFirstMarkedOnLine( int y, int index, unsigned short halfWidth, sCell** ppCells ) const;
    int FindLastMarkedOnLine( int y, int index, unsigned short halfWidth, sCell** ppCells ) const;
    short GetMaxElevationInCell( const geometry::Point2f& center, int floodElevation ) const;
    //@}

private:
    //! @name Member data
    //@{
    static const int cellWidth_;
    const ElevationGetter_ABC& getter_;
    //@}
};

}

#endif // __FloodModel_h_
