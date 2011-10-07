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

#include <geometry/types.h>

namespace flood
{
    class ElevationGetter_ABC;

// =============================================================================
/** @class  FloodModel
    @brief  FloodModel
*/
// Created: JSR 2010-12-08
// =============================================================================
class FloodModel
{
public:
    //! @name Types
    //@{
    typedef std::vector< geometry::Polygon2f* > T_Polygons;
    typedef T_Polygons::const_iterator        CIT_Polygons;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit FloodModel( const ElevationGetter_ABC& getter );
    virtual ~FloodModel();
    //@}

    //! @name Copy/Assignment
    //@{
    FloodModel& operator=( const FloodModel& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    void GenerateFlood( const geometry::Point2f& center, int depth, int refDist, bool force = false );
    void GenerateFlood( const geometry::Point2d& center, int depth, int refDist, bool force = false );
    const geometry::Point2f& GetCenter() const { return center_; }
    int GetReferenceDistance() const { return refDist_; }
    int GetDepth() const { return oldDepth_; }
    const T_Polygons& GetDeepAreas() const { return deepAreas_; }
    const T_Polygons& GetLowAreas() const { return lowAreas_; }
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
    //! @name Copy/Assignment
    //@{
    FloodModel( const FloodModel& );            //!< Copy constructor
    //@}

private:
    //! @name Helpers
    //@{
    void Propagate( int floodElevation );
    bool FindFirstUnmarkedCell( int& xRet, int& yRet ) const;
    bool FindFirstMarkedCell( int& xRet, int& yRet, int index ) const;
    int MarkAdjacentCells( int x, int y, bool deep, int nPolygonIndex );
    void MarkCells( int xStart, int yStart, int nPolygonIndex );
    void CreatePolygon( int nPolygonIndex );
    int FindUnmarkedOnNextLine( int xStart, int xEnd, int y, bool deep ) const;
    int FindFirstMarkedOnLine( int y, int index ) const;
    int FindLastMarkedOnLine( int y, int index ) const;
    void Reset();
    //@}

private:
    //! @name Member data
    //@{
    static const int cellWidth_;
    const ElevationGetter_ABC& getter_;
    geometry::Point2f center_;
    T_Polygons deepAreas_;
    T_Polygons lowAreas_;
    int refDist_;
    int oldDepth_;
    sCell** ppCells_;
    unsigned short halfWidth_;
    //@}
};

}

#endif // __FloodModel_h_
