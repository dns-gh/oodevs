// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLimit_h_
#define __ActionParameterLimit_h_

#include "Location.h"

namespace sword
{
    class Line;
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
             Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter );
             Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const sword::Line& line );
             Limit( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
    virtual ~Limit();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}
};

}
}

#endif // __ActionParameterLimit_h_
