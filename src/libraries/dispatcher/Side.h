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

#include "game_asn/Simulation.h"
#include "Sendable.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Karma.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Formation_ABC;
    class KnowledgeGroup_ABC;
    class ModelVisitor_ABC;
    class Object_ABC;
    class Population_ABC;
}
namespace dispatcher
{
    class Model_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  Side
    @brief  Side
*/
// Created: NLD 2006-09-19
// =============================================================================
class Side : public Sendable< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Side( const Model_ABC& model, const ASN1T_MsgTeamCreation& msg );
    virtual ~Side();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const ASN1T_MsgChangeDiplomacy&    asnMsg );
    void Update( const ASN1T_MsgChangeDiplomacyAck& asnMsg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual const kernel::Karma& GetKarma() const;
    virtual void Register( kernel::Formation_ABC& formation );
    virtual void Remove( kernel::Formation_ABC& formation );
    virtual void Register( kernel::Population_ABC& population );
    virtual void Remove( kernel::Population_ABC& population );
    virtual void Register( kernel::Object_ABC& object );
    virtual void Remove( kernel::Object_ABC& object );
    virtual void Register( kernel::KnowledgeGroup_ABC& knGroup );
    virtual void Remove( kernel::KnowledgeGroup_ABC& knGroup );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Side( const Side& );            //!< Copy constructor
    Side& operator=( const Side& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Team_ABC*, ASN1T_EnumDiplomacy > T_Diplomacies;
    //@}

private:
    //! @name Member data
    //@{
    const Model_ABC&        model_;

public:
    const std::string   name_;
    ASN1T_EnumDiplomacy nType_;
    kernel::Karma karma_;
    T_Diplomacies       diplomacies_;
    tools::Resolver< kernel::KnowledgeGroup_ABC > knowledgeGroups_;
    tools::Resolver< kernel::Formation_ABC >      formations_;
    tools::Resolver< kernel::Object_ABC >         objects_;
    tools::Resolver< kernel::Population_ABC >     populations_;
    //@}
};

}

#endif // __Side_h_
