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
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterObstacle( const QString& name, const kernel::ObjectType& type );
             ActionParameterObstacle( const kernel::OrderParameter& parameter, const kernel::ObjectType& type );
             ActionParameterObstacle( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const ASN1T_MissionGenObject& asn );
             ActionParameterObstacle( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, const ASN1T_MissionGenObject& asn );
             ActionParameterObstacle( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, xml::xistream& xis );
             ActionParameterObstacle( const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::ObjectType >& types, xml::xistream& xis );
    virtual ~ActionParameterObstacle();
    //@}

    //! @name Operations
    //@{
    virtual QString GetType() const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    void CommitTo( ASN1T_MissionGenObject& asn ) const;
    void Clean( ASN1T_MissionGenObject& asn ) const;

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
    virtual void Serialize( xml::xostream& xos ) const;
    void SetParameters( const ASN1T_MissionGenObject& asn );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ObjectType& type_;
    //@}
};

#endif // __ActionParameterObstacle_h_
