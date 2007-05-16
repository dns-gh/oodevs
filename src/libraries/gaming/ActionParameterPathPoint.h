// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPathPoint_h_
#define __ActionParameterPathPoint_h_

#include "ActionParameterLocation.h"

// =============================================================================
/** @class  ActionParameterPathPoint
    @brief  ActionParameterPathPoint
*/
// Created: SBO 2007-05-15
// =============================================================================
class ActionParameterPathPoint : public ActionParameterLocation
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterPathPoint( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             ActionParameterPathPoint( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             ActionParameterPathPoint( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ActionParameterPathPoint();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterPathPoint( const ActionParameterPathPoint& );            //!< Copy constructor
    ActionParameterPathPoint& operator=( const ActionParameterPathPoint& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterPathPoint_h_
