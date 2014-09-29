// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationsLayer_h_
#define __LocationsLayer_h_

#include "Layer.h"

namespace gui
{
    class Controllers;
}

namespace gui
{
    class GlTools_ABC;

// =============================================================================
/** @class  LocationsLayer
    @brief  LocationsLayer
*/
// Created: SBO 2007-03-28
// =============================================================================
class LocationsLayer : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LocationsLayer( kernel::Controllers& controllers, const gui::GlTools_ABC& tools );
    virtual ~LocationsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( Viewport_ABC& viewport );
    void AddLocation( const geometry::Point2f& point );
    void Reset();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< geometry::Point2f > T_Locations;
    typedef T_Locations::const_iterator    CIT_Locations;
    //@}

private:
    //! @name Member data
    //@{
    const gui::GlTools_ABC& tools_;
    T_Locations locations_;
    //@}
};

}

#endif // __LocationsLayer_h_
