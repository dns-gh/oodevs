// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Viewport3d_h_
#define __Viewport3d_h_

#include "clients_gui/Viewport_ABC.h"

class ViewFrustum;

namespace gui
{

// =============================================================================
/** @class  Viewport3d
    @brief  Viewport3d
*/
// Created: AGE 2007-02-23
// =============================================================================
class Viewport3d : public Viewport_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Viewport3d( const ViewFrustum& frustum );
    virtual ~Viewport3d();
    //@}

    //! @name Operations
    //@{
    virtual bool IsVisible( const geometry::Point2f& point ) const;
    virtual bool IsVisible( const geometry::Rectangle2f& rectangle ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Viewport3d( const Viewport3d& );            //!< Copy constructor
    Viewport3d& operator=( const Viewport3d& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const ViewFrustum& frustum_;
    //@}
};

}

#endif // __Viewport3d_h_
