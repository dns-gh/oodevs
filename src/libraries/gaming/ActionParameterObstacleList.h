// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObstacleList_h_
#define __ActionParameterObstacleList_h_

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
/** @class  ActionParameterObstacleList
    @brief  ActionParameterObstacleList
*/
// Created: SBO 2007-04-16
// =============================================================================
class ActionParameterObstacleList : public ActionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterObstacleList( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const ASN1T_ListMissionGenObject& asn );
    virtual ~ActionParameterObstacleList();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterObstacleList( const ActionParameterObstacleList& );            //!< Copy constructor
    ActionParameterObstacleList& operator=( const ActionParameterObstacleList& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterObstacleList_h_
