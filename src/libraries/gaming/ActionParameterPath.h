// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPath_h_
#define __ActionParameterPath_h_

#include "ActionParameterLocation.h"

namespace kernel
{
    class Entity_ABC;
    class OrderParameter;
}

// =============================================================================
/** @class  ActionParameterPath
    @brief  ActionParameterPath
*/
// Created: SBO 2007-04-26
// =============================================================================
class ActionParameterPath : public ActionParameterLocation
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterPath( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location, const kernel::Entity_ABC& entity );
             ActionParameterPath( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn, const kernel::Entity_ABC& entity );
             ActionParameterPath( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location, const kernel::Entity_ABC& entity );
             ActionParameterPath( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn, const kernel::Entity_ABC& entity );
    virtual ~ActionParameterPath();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterPath( const ActionParameterPath& );            //!< Copy constructor
    ActionParameterPath& operator=( const ActionParameterPath& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterPath_h_
