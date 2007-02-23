// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DebugPoints_h_
#define __DebugPoints_h_

#include "DIN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

// =============================================================================
/** @class  DebugPoints
    @brief  DebugPoints
*/
// Created: AGE 2006-02-13
// =============================================================================
class DebugPoints : public kernel::Extension_ABC
                  , public kernel::Updatable_ABC< DebugPointsMessage >
                  , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DebugPoints();
    virtual ~DebugPoints();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DebugPoints( const DebugPoints& );            //!< Copy constructor
    DebugPoints& operator=( const DebugPoints& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const DebugPointsMessage& message );
    //@}

private:
    //! @name Member data
    //@{
    T_PointVector points_;
    //@}
};

#endif // __DebugPoints_h_
