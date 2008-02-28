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

#include "game_asn/Simulation.h"
#include "ActionParameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class ObjectType;
    class Automat_ABC;
}

// =============================================================================
/** @class  ActionParameterObstacle
    @brief  ActionParameterObstacle
*/
// Created: SBO 2007-04-16
// =============================================================================
class ActionParameterObstacle : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterObstacle( const kernel::OrderParameter& parameter, const kernel::ObjectType& type );
             ActionParameterObstacle( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, const ASN1T_PlannedWork& asn );
             ActionParameterObstacle( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, xml::xistream& xis );
             ActionParameterObstacle( const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, xml::xistream& xis );
    virtual ~ActionParameterObstacle();
    //@}

    //! @name Operations
    //@{
    void AddObstacleType( unsigned int type );

    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    void CommitTo( ASN1T_PlannedWork& asn ) const;
    void Clean( ASN1T_PlannedWork& asn ) const;

    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterObstacle( const ActionParameterObstacle& );            //!< Copy constructor
    ActionParameterObstacle& operator=( const ActionParameterObstacle& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats );
    virtual void Serialize( xml::xostream& xos ) const;
    void SetParameters( const ASN1T_PlannedWork& asn, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ObjectType& type_;
    //@}
};

#endif // __ActionParameterObstacle_h_
