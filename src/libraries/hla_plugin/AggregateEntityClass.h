// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_AggregateEntityClass_h
#define plugins_hla_AggregateEntityClass_h

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
    class Aggregate_ABC;
    class AggregateFactory_ABC;
    class AgentSubject_ABC;
    class Federate_ABC;
    class ClassBuilder_ABC;
    class RemoteAggregateFactory_ABC;
    class RemoteAgentListener_ABC;

// =============================================================================
/** @class  AggregateEntityClass
    @brief  Aggregate entity class
*/
// Created: AGE 2008-02-22
// =============================================================================
class AggregateEntityClass : private AgentListener_ABC
                           , private ::hla::ObjectRegistration_ABC< Aggregate_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AggregateEntityClass( Federate_ABC& federate, AgentSubject_ABC& subject,
                                   const AggregateFactory_ABC& factory, const RemoteAggregateFactory_ABC& remoteFactory,
                                   const ClassBuilder_ABC& builder );
    virtual ~AggregateEntityClass();
    //@}

    //! @name Operations
    //@{
    void Register( RemoteAgentListener_ABC& listener );
    void Unregister( RemoteAgentListener_ABC& listener );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Created( Agent_ABC& agent, const std::string& identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
    virtual void Destroyed( const std::string& identifier );
    //@}

    //! @name Operations
    //@{
    virtual Aggregate_ABC& Create( const ::hla::ObjectIdentifier& objectID, const std::string& objectName );
    virtual void Destroy( Aggregate_ABC& object );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< Aggregate_ABC > T_Entity;
    typedef std::map< std::string, T_Entity > T_Entities;
    typedef std::vector< RemoteAgentListener_ABC* > T_Listeners;
    //@}

private:
    //! @name Member data
    //@{
    unsigned short id_;
    AgentSubject_ABC& subject_;
    const AggregateFactory_ABC& factory_;
    const RemoteAggregateFactory_ABC& remoteFactory_;
    T_Entities localEntities_;
    T_Entities remoteEntities_;
    T_Listeners listeners_;
    std::auto_ptr< ::hla::Class< Aggregate_ABC > > hlaClass_;
    //@}
};

}
}

#endif // plugins_hla_AggregateEntityClass_h
