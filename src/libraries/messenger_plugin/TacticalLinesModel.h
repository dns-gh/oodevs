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

#include "game_asn/Messenger.h"
#include "clients_kernel/Resolver.h"

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
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
    @brief  TacticalLinesModel
*/
// Created: NLD 2006-11-10
// =============================================================================
class TacticalLinesModel
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLinesModel( dispatcher::ClientPublisher_ABC& clients, IdManager& idManager, const kernel::CoordinateConverter_ABC& converter );
    virtual ~TacticalLinesModel();
    //@}


    //! @name Network
    //@{
    void HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgLimitCreationRequest&    asn );
    void HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgLimitDestructionRequest& asn );
    void HandleLimitRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgLimitUpdateRequest&      asn );
    void HandleLimaRequest ( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgLimaCreationRequest&     asn );
    void HandleLimaRequest ( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgLimaDestructionRequest&  asn );
    void HandleLimaRequest ( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgLimaUpdateRequest&       asn );
    void SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name xml read / write
    //@{
    void ReadLima ( xml::xistream& xis, const ASN1T_TacticalLinesDiffusion& diffusion );
    void ReadLimit( xml::xistream& xis, const ASN1T_TacticalLinesDiffusion& diffusion );
    void Write    ( xml::xostream& xos ) const;
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

    //! @name Copy/Assignement
    //@{
    TacticalLinesModel( const TacticalLinesModel& );            //!< Copy constructor
    TacticalLinesModel& operator=( const TacticalLinesModel& ); //!< Assignement operator
    //@}

private:
    //! @name Members
    //@{
    dispatcher::ClientPublisher_ABC& clients_;
    IdManager& idManager_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Resolver< Limit > limits_;
    kernel::Resolver< Lima  > limas_;
    //@}

};

}
}

#endif // __TacticalLinesModel_h_
