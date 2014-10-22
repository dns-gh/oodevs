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

#include <geometry/types.h>
#include <boost/noncopyable.hpp>
#include <vector>

namespace propagation
{
    class FloodModel_ABC;
}

namespace gui
{
    class GLView_ABC;

// =============================================================================
/** @class  FloodDrawer
    @brief  Flood drawer
*/
// Created: JSR 2010-12-21
// =============================================================================
class FloodDrawer : boost::noncopyable
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
    void Draw( GLView_ABC& view );
    void Reset( const propagation::FloodModel_ABC& model, const geometry::Point2f& point, int depth, int refDist );
    //@}

    //! @name Accessors
    //@{
    const geometry::Point2f& GetCenter() const;
    int GetReferenceDistance() const;
    int GetDepth() const;
    //@}

private:
    //! @name Helpers
    //@{
    void Render() const;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< geometry::Polygon2f > deepAreas_;
    std::vector< geometry::Polygon2f > lowAreas_;
    const geometry::Point2f point_;
    unsigned int callListId_;
    float alpha_;
    int depth_;
    int refDist_;
    //@}
};

}

#endif // __FloodDrawer_h_
