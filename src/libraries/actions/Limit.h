// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLimit_h_
#define __ActionParameterLimit_h_

#include "Location.h"

namespace Common
{
    class MsgLine;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Limit
    @brief  Limit
*/
// Created: SBO 2007-04-13
// =============================================================================
class Limit : public Location
{
public:
    //! @name Constructors/Destructor
    //@{
             Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Common::MsgLine& line );
             Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~Limit();
    //@}

    //! @name Operations
    //@{
    void CommitTo( Common::MsgMissionParameter& message ) const;
    void Clean( Common::MsgMissionParameter& message ) const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Limit( const Limit& );            //!< Copy constructor
    Limit& operator=( const Limit& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __ActionParameterLimit_h_
