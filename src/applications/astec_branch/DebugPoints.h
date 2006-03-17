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
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Drawable_ABC.h"

// =============================================================================
/** @class  DebugPoints
    @brief  DebugPoints
*/
// Created: AGE 2006-02-13
// =============================================================================
class DebugPoints : public Extension_ABC
                  , public Updatable_ABC< DebugPointsMessage >
                  , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DebugPoints();
    virtual ~DebugPoints();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
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
