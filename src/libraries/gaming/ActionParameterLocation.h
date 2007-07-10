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

#include "ActionParameter.h"
#include "Location.h"

// =============================================================================
/** @class  ActionParameterLocation
    @brief  ActionParameterLocation
*/
// Created: SBO 2007-04-19
// =============================================================================
class ActionParameterLocation : public ActionParameter< QString >
                              , public Location
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterLocation( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             ActionParameterLocation( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Location& asn );
             ActionParameterLocation( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
             ActionParameterLocation( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~ActionParameterLocation();
    //@}

    //! @name Accessors
    //@{
    virtual geometry::Point2f GetPosition() const;
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;

    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    void CommitTo( ASN1T_Location& asn ) const;
    void Clean( ASN1T_Location& asn ) const;

    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLocation( const ActionParameterLocation& );            //!< Copy constructor
    ActionParameterLocation& operator=( const ActionParameterLocation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

#endif // __ActionParameterLocation_h_
