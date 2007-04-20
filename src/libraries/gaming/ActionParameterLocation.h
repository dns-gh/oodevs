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
#include "LocationPositions.h"

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  ActionParameterLocation
    @brief  ActionParameterLocation
*/
// Created: SBO 2007-04-19
// =============================================================================
class ActionParameterLocation : public ActionParameter< QString >
                              , public LocationPositions
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterLocation( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn );
             ActionParameterLocation( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn, const kernel::Entity_ABC& entity );
    virtual ~ActionParameterLocation();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLocation( const ActionParameterLocation& );            //!< Copy constructor
    ActionParameterLocation& operator=( const ActionParameterLocation& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterLocation_h_
