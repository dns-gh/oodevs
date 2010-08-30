// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __IntelligencesModel_h_
#define __IntelligencesModel_h_

#include "tools/Resolver.h"
#include "protocol/protocol.h"

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
    class Intelligence;
    class IdManager;
    class Entity_ABC;

// =============================================================================
/** @class  IntelligencesModel
    @brief  IntelligencesModel
*/
// Created: RDS 2008-04-07
// =============================================================================
class IntelligencesModel : public tools::Resolver< Intelligence >
{
public:
    //! @name Constructors/Destructor
    //@{
             IntelligencesModel( dispatcher::ClientPublisher_ABC& clients, IdManager& idManager, const kernel::CoordinateConverter_ABC& converter );
    virtual ~IntelligencesModel();
    //@}

    //! @name Network
    //@{
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgsClientToMessenger::MsgIntelligenceCreationRequest&     asn );
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgsClientToMessenger::MsgIntelligenceUpdateRequest&       asn );
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const MsgsClientToMessenger::MsgIntelligenceDestructionRequest&  asn );
    void SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name xml read / write
    //@{
    void ReadIntelligence( xml::xistream& xis, const Common::FormationId& formation );
    void Write           ( xml::xostream& xos );
    //@}

    typedef std::map< unsigned int , std::set< const Entity_ABC* > > T_EntityCollectorMap ;
    typedef T_EntityCollectorMap                               T_FormationMap ;
    void CollectFormations(T_FormationMap&);

private:
    //! @name Copy/Assignment
    //@{
    IntelligencesModel( const IntelligencesModel& );            //!< Copy constructor
    IntelligencesModel& operator=( const IntelligencesModel& ); //!< Assignment operator
    //@}

private:
    //! @name Members
    //@{
    dispatcher::ClientPublisher_ABC& clients_;
    IdManager& idManager_;
    const kernel::CoordinateConverter_ABC& converter_ ;
    //@}
};

}
}

#endif // __IntelligencesModel_h_
