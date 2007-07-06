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

#include "game_asn/asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  DebugPoints
    @brief  DebugPoints
*/
// Created: AGE 2006-02-13
// =============================================================================
class DebugPoints : public kernel::Extension_ABC
                  , public kernel::Updatable_ABC< ASN1T_MsgDebugPoints >
                  , public kernel::Drawable_ABC
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
    //! @name Copy/Assignement
    //@{
    DebugPoints( const DebugPoints& );            //!< Copy constructor
    DebugPoints& operator=( const DebugPoints& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgDebugPoints& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_PointVector points_;
    //@}
};

#endif // __DebugPoints_h_
