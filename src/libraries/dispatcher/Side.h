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
#include <tools/Resolver.h>
#include <tools/Map.h>

namespace sword
{
    enum EnumDiplomacy;
    class ChangeDiplomacy;
    class ChangeDiplomacyAck;
    class PartyCreation;
    class RgbColor;
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
           , public kernel::Updatable_ABC< sword::ChangeDiplomacy >
           , public kernel::Updatable_ABC< sword::ChangeDiplomacyAck >
{
public:
    //! @name Constructors/Destructor
    //@{
             Side( const Model_ABC& model, const sword::PartyCreation& msg );
    virtual ~Side();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::ChangeDiplomacy& asnMsg );
    virtual void DoUpdate( const sword::ChangeDiplomacyAck& asnMsg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual const kernel::Karma& GetKarma() const;
    virtual bool GetExtension( const std::string& key, std::string& result ) const;

    virtual void Register( dispatcher::Formation_ABC& formation );
    virtual void Remove( dispatcher::Formation_ABC& formation );
    virtual void Register( dispatcher::Population_ABC& population );
    virtual void Remove( dispatcher::Population_ABC& population );
    virtual void Register( dispatcher::Inhabitant_ABC& inhabitant );
    virtual void Remove( dispatcher::Inhabitant_ABC& inhabitant );
    virtual void Register( dispatcher::Object_ABC& object );
    virtual void Remove( dispatcher::Object_ABC& object );
    virtual void Register( dispatcher::KnowledgeGroup_ABC& knGroup );
    virtual void Remove( dispatcher::KnowledgeGroup_ABC& knGroup );
    //@}

private:
    //! @name Types
    //@{
    typedef tools::Map< const kernel::Team_ABC*, sword::EnumDiplomacy > T_Diplomacies;
    //@}

private:
    //! @name Member data
    //@{
    const Model_ABC& model_;
    sword::EnumDiplomacy nType_;
    kernel::Karma karma_;
    std::unique_ptr< sword::RgbColor > color_;
    T_Diplomacies diplomacies_;
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups_;
    tools::Resolver< dispatcher::Formation_ABC >      formations_;
    tools::Resolver< dispatcher::Object_ABC >         objects_;
    tools::Resolver< dispatcher::Population_ABC >     populations_;
    tools::Resolver< dispatcher::Inhabitant_ABC >     inhabitants_;
    std::map< std::string, std::string >              extensions_;
    //@}
};

}

#endif // __Side_h_
