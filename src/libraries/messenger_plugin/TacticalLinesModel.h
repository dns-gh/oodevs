// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLinesModel_h_
#define __TacticalLinesModel_h_

#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace sword
{
    class Diffusion;
    class LimitCreationRequest;
    class LimitDestructionRequest;
    class LimitUpdateRequest;
    class PhaseLineCreationRequest;
    class PhaseLineDestructionRequest;
    class PhaseLineUpdateRequest;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace plugins
{
namespace messenger
{
    class Lima;
    class Limit;
    class IdManager;
    class Entity_ABC;

// =============================================================================
/** @class  TacticalLinesModel
    @brief  Tactical lines model
*/
// Created: NLD 2006-11-10
// =============================================================================
class TacticalLinesModel : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinesModel( dispatcher::ClientPublisher_ABC& clients, IdManager& idManager, const kernel::CoordinateConverter_ABC& converter );
    virtual ~TacticalLinesModel();
    //@}

    //! @name Network
    //@{
    void HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::LimitCreationRequest& message );
    void HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::LimitDestructionRequest& message );
    void HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::LimitUpdateRequest& message );
    void HandleLimaRequest ( dispatcher::ClientPublisher_ABC& publisher, const sword::PhaseLineCreationRequest& message );
    void HandleLimaRequest ( dispatcher::ClientPublisher_ABC& publisher, const sword::PhaseLineDestructionRequest& message );
    void HandleLimaRequest ( dispatcher::ClientPublisher_ABC& publisher, const sword::PhaseLineUpdateRequest& message );
    void SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name xml read / write
    //@{
    void ReadLima ( xml::xistream& xis, const sword::Diffusion& diffusion );
    void ReadLimit( xml::xistream& xis, const sword::Diffusion& diffusion );
    void Write( xml::xostream& xos ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int , std::set< const Entity_ABC* > > T_TacticalLineCollectorMap ;
    typedef T_TacticalLineCollectorMap T_FormationMap ;
    typedef T_TacticalLineCollectorMap T_AutomatMap ;
    //@}

    //! @name Operations
    //@{
    void CollectFormations( T_FormationMap& );
    void CollectAutomats( T_AutomatMap& );
    //@}

private:
    //! @name Members
    //@{
    dispatcher::ClientPublisher_ABC& clients_;
    IdManager& idManager_;
    const kernel::CoordinateConverter_ABC& converter_;
    tools::Resolver< Limit > limits_;
    tools::Resolver< Lima  > limas_;
    //@}
};

}
}

#endif // __TacticalLinesModel_h_
