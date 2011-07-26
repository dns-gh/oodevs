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

// =============================================================================
/** @class  AggregateEntityClass
    @brief  Aggregate entity class
*/
// Created: AGE 2008-02-22
// =============================================================================
class AggregateEntityClass : private AgentListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AggregateEntityClass( Federate_ABC& federate, AgentSubject_ABC& subject,
                                   const AggregateFactory_ABC& factory, const ClassBuilder_ABC& builder );
    virtual ~AggregateEntityClass();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Created( Agent_ABC& agent, const std::string& identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
    virtual void Destroyed( const std::string& identifier );
    //@}

private:
    //! @name Types
    //@{
    struct UnitRegistration;
    typedef boost::shared_ptr< Aggregate_ABC > T_Entity;
    //@}

private:
    //! @name Member data
    //@{
    unsigned short id_;
    AgentSubject_ABC& subject_;
    const AggregateFactory_ABC& factory_;
    std::auto_ptr< UnitRegistration > registration_;
    std::auto_ptr< ::hla::Class< Aggregate_ABC > > hlaClass_;
    std::map< std::string, T_Entity > entities_;
    //@}
};

}
}

#endif // plugins_hla_AggregateEntityClass_h
