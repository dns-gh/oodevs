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

namespace flood
{
class FloodModel;

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
             FloodDrawer( const FloodModel& model );
    virtual ~FloodDrawer();
    //@}

    //! @name Operations
    //@{
    void Draw() const;
    void ResetTexture();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodDrawer( const FloodDrawer& );            //!< Copy constructor
    FloodDrawer& operator=( const FloodDrawer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void RenderTexture();
    void DrawPolygons( const std::vector< geometry::Polygon2f* >& polygons ) const;
    //@}

private:
    //! @name Member data
    //@{
    const FloodModel& model_;
    mutable unsigned int callListId_;
    //@}
};

}

#endif // __FloodDrawer_h_
