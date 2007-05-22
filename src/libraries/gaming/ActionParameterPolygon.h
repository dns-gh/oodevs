// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPolygon_h_
#define __ActionParameterPolygon_h_

#include "ActionParameterLocation.h"

// =============================================================================
/** @class  ActionParameterPolygon
    @brief  ActionParameterPolygon
*/
// Created: SBO 2007-05-22
// =============================================================================
class ActionParameterPolygon : public ActionParameterLocation
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterPolygon( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             ActionParameterPolygon( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Polygon& asn );
             ActionParameterPolygon( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~ActionParameterPolygon();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterPolygon( const ActionParameterPolygon& );            //!< Copy constructor
    ActionParameterPolygon& operator=( const ActionParameterPolygon& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterPolygon_h_
