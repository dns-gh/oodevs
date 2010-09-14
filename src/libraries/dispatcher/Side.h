// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Side_h_
#define __Side_h_

#include "Team_ABC.h"
#include "clients_kernel/Karma.h"
#include "tools/Resolver.h"

namespace Common
{
    class MsgChangeDiplomacy;
    enum EnumDiplomacy;
}

namespace MsgsSimToClient
{
    class MsgTeamCreation;
    class MsgChangeDiplomacyAck;
}

namespace dispatcher
{
    class Model_ABC;

// =============================================================================
/** @class  Side
    @brief  Side
*/
// Created: NLD 2006-09-19
// =============================================================================
class Side : public dispatcher::Team_ABC
           , public kernel::Extension_ABC
           , public kernel::Updatable_ABC< Common::MsgChangeDiplomacy >
           , public kernel::Updatable_ABC< MsgsSimToClient::MsgChangeDiplomacyAck >
{
public:
    //! @name Constructors/Destructor
    //@{
             Side( const Model_ABC& model, const MsgsSimToClient::MsgTeamCreation& msg );
    virtual ~Side();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const Common::MsgChangeDiplomacy& asnMsg );
    virtual void DoUpdate( const MsgsSimToClient::MsgChangeDiplomacyAck& asnMsg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual const kernel::Karma& GetKarma() const;

    virtual void Register( dispatcher::Formation_ABC& formation );
    virtual void Remove( dispatcher::Formation_ABC& formation );
    virtual void Register( dispatcher::Population_ABC& population );
    virtual void Remove( dispatcher::Population_ABC& population );
    virtual void Register( dispatcher::Object_ABC& object );
    virtual void Remove( dispatcher::Object_ABC& object );
    virtual void Register( dispatcher::KnowledgeGroup_ABC& knGroup );
    virtual void Remove( dispatcher::KnowledgeGroup_ABC& knGroup );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Side( const Side& );            //!< Copy constructor
    Side& operator=( const Side& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Team_ABC*, Common::EnumDiplomacy > T_Diplomacies;
    //@}

private:
    //! @name Member data
    //@{
    const Model_ABC& model_;
    const std::string name_;
    Common::EnumDiplomacy nType_;
    kernel::Karma karma_;
    T_Diplomacies diplomacies_;
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups_;
    tools::Resolver< dispatcher::Formation_ABC >      formations_;
    tools::Resolver< dispatcher::Object_ABC >         objects_;
    tools::Resolver< dispatcher::Population_ABC >     populations_;
    //@}
};

}

#endif // __Side_h_
