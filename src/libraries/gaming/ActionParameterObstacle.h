// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObstacle_h_
#define __ActionParameterObstacle_h_

#include "game_asn/Asn.h"
#include "ActionParameter.h"
#include "LocationPositions.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class ObjectType;
}

// =============================================================================
/** @class  ActionParameterObstacle
    @brief  ActionParameterObstacle
*/
// Created: SBO 2007-04-16
// =============================================================================
class ActionParameterObstacle : public ActionParameter< QString >
                              , public LocationPositions
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterObstacle( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const ASN1T_MissionGenObject& asn );
    virtual ~ActionParameterObstacle();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterObstacle( const ActionParameterObstacle& );            //!< Copy constructor
    ActionParameterObstacle& operator=( const ActionParameterObstacle& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::ObjectType& type_;
    //@}
};

#endif // __ActionParameterObstacle_h_
