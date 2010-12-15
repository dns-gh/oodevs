// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodHelper_h_
#define __FloodHelper_h_

#include <geometry/types.h>
#include <boost/noncopyable.hpp>

namespace flood
{
    class ElevationGetter_ABC;

// =============================================================================
/** @class  FloodHelper
    @brief  FloodHelper
*/
// Created: JSR 2010-12-08
// =============================================================================
class FloodHelper : boost::noncopyable // $$$$ MCO : crappy name ! why "Helper" ?
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FloodHelper( const ElevationGetter_ABC& getter );
    virtual ~FloodHelper();
    //@}

    //! @name Operations
    //@{
    void GenerateFlood( const geometry::Point2f& center, int depth, int refDist ); // $$$$ MCO : where are the unit tests ?!

    void Draw() const; // $$$$ MCO : separation of concerns
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
    typedef std::vector< geometry::Polygon2f* > T_Polygons;
    typedef T_Polygons::const_iterator        CIT_Polygons;
    //@}

private:
    //! @name Helpers
    //@{
    // $$$$ MCO : so many methods -> code smell the class does to much things
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
    void RenderTexture();
    void DrawPolygons( const T_Polygons& polygons ) const;
    //@}

private:
    //! @name Member data
    //@{
    // $$$$ MCO : so many member data -> code smell the class is too big
    static const int cellWidth_;
    const ElevationGetter_ABC& getter_;
    geometry::Point2f center_;
    T_Polygons deepAreas_;
    T_Polygons lowAreas_;
    int refDist_;
    int oldDepth_;
    sCell** ppCells_;
    unsigned int textureId_;
    unsigned short halfWidth_;
    //@}
};

}

#endif // __FloodHelper_h_
