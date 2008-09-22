// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObstacleList_h_
#define __ActionParameterObstacleList_h_

#include "game_asn/Simulation.h"
#include "Parameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class ObjectType;
    class Automat_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  ObstacleList
    @brief  ObstacleList
*/
// Created: SBO 2007-04-16
// =============================================================================
class ObstacleList : public Parameter< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObstacleList( const kernel::OrderParameter& parameter );
             ObstacleList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& resolver, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, const ASN1T_PlannedWorkList& asn );
             ObstacleList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& resolver, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, xml::xistream& xis );
    virtual ~ObstacleList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObstacleList( const ObstacleList& );            //!< Copy constructor
    ObstacleList& operator=( const ObstacleList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadObstacle( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& resolver, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats );
    //@}
};

    }
}

#endif // __ActionParameterObstacleList_h_
