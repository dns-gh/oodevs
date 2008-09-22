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
#include "SimpleEntity.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Resolver.h"

namespace dispatcher
{
    class Model;
    class ModelVisitor_ABC;
    class ClientPublisher_ABC;
    class KnowledgeGroup;
    class Formation;
    class Object;
    class Population;

// =============================================================================
/** @class  Side
    @brief  Side
*/
// Created: NLD 2006-09-19
// =============================================================================
class Side : public SimpleEntity< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Side( const Model& model, const ASN1T_MsgTeamCreation& msg );
    virtual ~Side();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const ASN1T_MsgChangeDiplomacy&    asnMsg );
    void Update( const ASN1T_MsgChangeDiplomacyAck& asnMsg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( ModelVisitor_ABC& visitor ) const;

    virtual bool IsFriend () const;
    virtual bool IsEnemy  () const;
    virtual bool IsNeutral() const;
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
    const Model&        model_;

public:
    const std::string   name_;
    ASN1T_EnumDiplomacy nType_;
    T_Diplomacies       diplomacies_;
    kernel::Resolver< KnowledgeGroup > knowledgeGroups_;
    kernel::Resolver< Formation >      formations_;
    kernel::Resolver< Object >         objects_;
    kernel::Resolver< Population >     populations_;
    //@}
};

}

#endif // __Side_h_
