// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_SurfaceVesselClass_h
#define plugins_hla_SurfaceVesselClass_h

#include "AgentListener_ABC.h"
#include <hla/ObjectRegistration_ABC.h>
#include <boost/shared_ptr.hpp>
#include <map>

namespace hla
{
    template< typename T > class Class;
}

namespace plugins
{
namespace hla
{
    class HlaObject_ABC;
    class HlaObjectFactory_ABC;
    class AgentSubject_ABC;
    class Federate_ABC;
    class ClassBuilder_ABC;
    class LocalAgentResolver_ABC;
    class ContextFactory_ABC;

// =============================================================================
/** @class  SurfaceVesselClass
    @brief  Surface vessel class
*/
// Created: SLI 2011-10-04
// =============================================================================
class SurfaceVesselClass : private AgentListener_ABC
                         , private ::hla::ObjectRegistration_ABC< HlaObject_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             SurfaceVesselClass( Federate_ABC& federate, AgentSubject_ABC& subject, LocalAgentResolver_ABC& resolver,
                                 const HlaObjectFactory_ABC& factory, const ClassBuilder_ABC& builder, const ContextFactory_ABC& identifierFactory );
    virtual ~SurfaceVesselClass();
    //@}

private:
    //! @name Operations
    //@{
    virtual void AggregateCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
    virtual void SurfaceVesselCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
    //@}

    //! @name Operations
    //@{
    virtual HlaObject_ABC& Create( const ::hla::ObjectIdentifier& objectID, const std::string& objectName );
    virtual void Destroy( HlaObject_ABC& object );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< HlaObject_ABC > T_Entity;
    typedef std::map< std::string, T_Entity > T_Entities;
    //@}

private:
    //! @name Member data
    //@{
    AgentSubject_ABC& subject_;
    LocalAgentResolver_ABC& resolver_;
    const HlaObjectFactory_ABC& factory_;
    const ContextFactory_ABC& identifierFactory_;
    T_Entities localEntities_;
    std::auto_ptr< ::hla::Class< HlaObject_ABC > > hlaClass_;
    //@}
};

}
}

#endif // plugins_hla_SurfaceVesselClass_h
