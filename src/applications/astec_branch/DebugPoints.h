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

// =============================================================================
/** @class  DebugPoints
    @brief  DebugPoints
*/
// Created: AGE 2006-02-13
// =============================================================================
class DebugPoints : public Extension_ABC
                  , public Updatable_ABC< DebugPointsMessage >
{

public:
    //! @name Constructors/Destructor
    //@{
             DebugPoints();
    virtual ~DebugPoints();
    //@}

    //! @name Operations
    //@{
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
    std::vector< geometry::Point2f > points_;
    //@}
};

#endif // __DebugPoints_h_
