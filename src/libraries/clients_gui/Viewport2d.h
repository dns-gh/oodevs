// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Viewport2d_h_
#define __Viewport2d_h_

#include "clients_kernel/Viewport_ABC.h"

namespace gui
{

// =============================================================================
/** @class  Viewport2d
    @brief  Viewport2d
*/
// Created: AGE 2007-02-23
// =============================================================================
class Viewport2d : public kernel::Viewport_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Viewport2d( const geometry::Rectangle2f& viewport );
    virtual ~Viewport2d();
    //@}

    //! @name Operations
    //@{
    virtual bool IsVisible( const geometry::Point2f& point ) const;
    virtual bool IsVisible( const geometry::Rectangle2f& rectangle ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Viewport2d( const Viewport2d& );            //!< Copy constructor
    Viewport2d& operator=( const Viewport2d& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const geometry::Rectangle2f& viewport_;
    //@}
};

}

#endif // __Viewport2d_h_
