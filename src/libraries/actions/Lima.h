// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLima_h_
#define __ActionParameterLima_h_

#include "Parameter.h"

namespace sword
{
    class PhaseLineOrder;
}

namespace kernel
{
    class Location_ABC;
    class CoordinateConverter_ABC;
}

namespace actions {
    namespace parameters {

class Location;

// =============================================================================
/** @class  Lima
    @brief  Lima
*/
// Created: SBO 2007-04-26
// =============================================================================
class Lima : public Parameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
             Lima( const kernel::OrderParameter& parameter );
             Lima( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Lima( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const sword::PhaseLineOrder& message );
             Lima( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~Lima();
    //@}

    //! @name Operations
    //@{
    void AddFunction( unsigned int i );
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;

    void CommitTo( sword::PhaseLineOrder& message ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    void Clean( sword::PhaseLineOrder& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    void ReadParameter( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

}
}

#endif // __ActionParameterLima_h_
