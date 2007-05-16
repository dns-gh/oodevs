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

#include "game_asn/Asn.h"
#include "ActionParameter.h"
#include "clients_kernel/LocationVisitor_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class OrderParameter;
    class CoordinateConverter_ABC;
    class Location_ABC;
}

// =============================================================================
/** @class  ActionParameterPath
    @brief  ActionParameterPath
*/
// Created: SBO 2007-04-26
// =============================================================================
class ActionParameterPath : public ActionParameter< QString >
                          , public kernel::LocationVisitor_ABC
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

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterPath( const ActionParameterPath& );            //!< Copy constructor
    ActionParameterPath& operator=( const ActionParameterPath& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& ) {}
    virtual void VisitCircle ( const geometry::Point2f&, float ) {}
    virtual void VisitPoint  ( const geometry::Point2f& ) {}
    void AddPoints( const ASN1T_Localisation& asn, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

#endif // __ActionParameterPath_h_
