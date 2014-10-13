// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLocation_h_
#define __ActionParameterLocation_h_

#include "Parameter.h"
#include "LocationBase.h"

namespace kernel {
    class CoordinateConverter_ABC;
    class Location_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Location
    @brief  Location
*/
// Created: SBO 2007-04-19
// =============================================================================
class Location : public Parameter< QString >
               , public LocationBase
{
public:
    //! @name Constructors/Destructor
    //@{
             Location( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter );
             Location( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Location( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const sword::Location& message );
    virtual ~Location();
    //@}

    //! @name Accessors
    //@{
    virtual geometry::Point2f GetPosition() const;
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const ::gui::Viewport_ABC& viewport, ::gui::GLView_ABC& tools ) const;
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;

    using LocationBase::CommitTo;
    using LocationBase::Clean;
    virtual void CommitTo( std::string& content ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

}
}

#endif // __ActionParameterLocation_h_
