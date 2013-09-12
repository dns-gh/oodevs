// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodDrawer_h_
#define __FloodDrawer_h_

#include <vector>
#include <geometry/types.h>

namespace propagation
{
    class FloodModel_ABC;
// =============================================================================
/** @class  FloodDrawer
    @brief  FloodDrawer
*/
// Created: JSR 2010-12-21
// =============================================================================
class FloodDrawer
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodDrawer();
             FloodDrawer( const propagation::FloodModel_ABC& model, const geometry::Point2f& point, int depth, int refDist );
    virtual ~FloodDrawer();
    //@}

    //! @name Operations
    //@{
    void Draw() const;
    void Reset( const propagation::FloodModel_ABC& model, const geometry::Point2f& point, int depth, int refDist );
    //@}

    //! @name Accessors
    //@{
    const geometry::Point2f& GetCenter() const { return point_; }
    int GetReferenceDistance() const { return refDist_; }
    int GetDepth() const { return depth_; }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodDrawer( const FloodDrawer& );            //!< Copy constructor
    FloodDrawer& operator=( const FloodDrawer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void RenderTexture( const std::vector< geometry::Polygon2f >& deepAreas, const std::vector< geometry::Polygon2f >& lowAreas );
    void ResetTexture();
    void DrawPolygons( const std::vector< geometry::Polygon2f >& polygons ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< geometry::Polygon2f > deepAreas_;
    std::vector< geometry::Polygon2f > lowAreas_;
    const geometry::Point2f point_;
    int depth_;
    int refDist_;
    mutable unsigned int callListId_;
    //@}
};

}

#endif // __FloodDrawer_h_
