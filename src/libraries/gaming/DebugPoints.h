// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DebugPoints_h_
#define __DebugPoints_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace sword
{
    class DebugPoints;
}

// =============================================================================
/** @class  DebugPoints
    @brief  DebugPoints
*/
// Created: AGE 2006-02-13
// =============================================================================
class DebugPoints : public kernel::Extension_ABC
                  , public kernel::Updatable_ABC< sword::DebugPoints >
                  , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DebugPoints( const kernel::CoordinateConverter_ABC& converter );
    virtual ~DebugPoints();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DebugPoints( const DebugPoints& );            //!< Copy constructor
    DebugPoints& operator=( const DebugPoints& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::DebugPoints& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_PointVector points_;
    //@}
};

#endif // __DebugPoints_h_
