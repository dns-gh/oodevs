// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledge_h_
#define __AgentKnowledge_h_

#include "ASN_Types.h"
#include "clients_kernel/IDManager.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "AgentKnowledge_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class Team_ABC;
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  AgentKnowledge
    @brief  Represents the knowledge a gtia has of an agent.
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentKnowledge : public kernel::EntityImplementation< AgentKnowledge_ABC >
                     , public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< ASN1T_MsgUnitKnowledgeUpdate >
                     , public kernel::Drawable_ABC
{

public:
    //! @name Constructor / Destructor
    //@{
             AgentKnowledge( const kernel::KnowledgeGroup_ABC& group, const ASN1T_MsgUnitKnowledgeCreation& message, kernel::Controller& controller, 
                             const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, const kernel::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~AgentKnowledge();
    //@}

    //! @name operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    bool IsInTeam( const kernel::Team_ABC& team ) const;
    bool KnowledgeIsInTeam( const kernel::Entity_ABC& team ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual QString GetTypeName() const;

    virtual const kernel::Agent_ABC* GetEntity() const;
    virtual const kernel::KnowledgeGroup_ABC& GetOwner() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message );
    //@}

public:
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    const kernel::Resolver_ABC< kernel::Team_ABC >& teamResolver_;
    const kernel::KnowledgeGroup_ABC& group_;

    kernel::Agent_ABC&      realAgent_;
    const kernel::Team_ABC* team_;
    
    // $$$$ AGE 2006-02-22: Use Attributes ?
    kernel::OptionalValue< std::string >                strPosition_;
    kernel::OptionalValue< uint >                       nDirection_;
    kernel::OptionalValue< uint >                       nSpeed_;
    kernel::OptionalValue< uint >                       nEtatOps_;
    kernel::OptionalValue< kernel::E_PerceptionResult > nCurrentPerceptionLevel_;
    kernel::OptionalValue< kernel::E_PerceptionResult > nMaxPerceptionLevel_;
    kernel::OptionalValue< E_NatureLevel >              nLevel_;
    kernel::OptionalValue< E_UnitNatureWeapon >         nWeapon_;
    kernel::OptionalValue< E_UnitNatureSpecialization > nSpecialization_;
    kernel::OptionalValue< E_UnitNatureQualifier >      nQualifier_;
    kernel::OptionalValue< E_UnitNatureCategory >       nCategory_;
    kernel::OptionalValue< E_UnitNatureMobility >       nMobility_;
    kernel::OptionalValue< E_UnitCapaciteMission >      nCapacity_;
    kernel::OptionalValue< bool >                       bIsPC_;
    kernel::OptionalValue< bool >                       bSurrendered_;
    kernel::OptionalValue< bool >                       bPrisonner_;
    kernel::OptionalValue< bool >                       bRefugies_;
    kernel::OptionalValue< uint >                       nRelevance_;
};

#endif // __AgentKnowledge_h_