// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_AgentProxy_h_
#define hla_plugin_AgentProxy_h_

#include "Agent_ABC.h"

namespace dispatcher
{
    class Agent_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  AgentProxy
    @brief  Agent proxy
*/
// Created: SLI 2011-02-04
// =============================================================================
class AgentProxy : public Agent_ABC
                 , private dispatcher::Observer< sword::UnitAttributes >
                 , private dispatcher::Observer< sword::UnitEnvironmentType >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentProxy( dispatcher::Agent_ABC& agent );
    virtual ~AgentProxy();
    //@}

    //! @name Getters
    //@{
    virtual std::string GetName() const;
    virtual geometry::Point2d GetPosition() const;
    virtual unsigned short GetAltitude() const;
    virtual unsigned short GetSpeed() const;
    virtual unsigned short GetDirection() const;
    virtual const kernel::Karma& GetForce() const;
    virtual const tools::Resolver< dispatcher::Equipment >& GetEquipments() const;
    //@}

private:
    //! @name Observer
    //@{
    virtual void Notify( const sword::UnitAttributes& attributes );
    virtual void Notify( const sword::UnitEnvironmentType& attributes );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Agent_ABC& agent_;
    //@}
};

}
}

#endif // hla_plugin_AgentProxy_h_
