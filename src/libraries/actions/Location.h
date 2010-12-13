// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
             Location( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Location( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const sword::MsgLocation& message );
             Location( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~Location();
    //@}

    //! @name Accessors
    //@{
    virtual geometry::Point2f GetPosition() const;
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;

    using LocationBase::CommitTo;
    using LocationBase::Clean;
    virtual void CommitTo( std::string& content ) const;
    virtual void CommitTo( sword::MsgMissionParameter& message ) const;
    virtual void CommitTo( sword::MsgMissionParameter_Value& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Location( const Location& );            //!< Copy constructor
    Location& operator=( const Location& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

    }
}

#endif // __ActionParameterLocation_h_
