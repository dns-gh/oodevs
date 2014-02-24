// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BaseDisplayer_h_
#define __BaseDisplayer_h_

#include "clients_kernel/Displayer_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Agent_ABC;
    class Object_ABC;
    class UrbanObject_ABC;
    class Team_ABC;
    class DotationType;
    class EquipmentType;
    class Inhabitant_ABC;
    class Population_ABC;
    class NBCAgent;
    class FireClass;
    class ComponentType;
    class BreakdownType;
    class Mission;
    class CoordinateConverter_ABC;
    class PopulationConcentration_ABC;
    class Entity_ABC;
    class Formation_ABC;
    class AgentType;
    class AutomatType;
    class PopulationType;
    class InhabitantType;
    class ObjectType;
    class KnowledgeGroupType;
    class HierarchyLevel_ABC;
    class Karma;
    class AgentKnowledge_ABC;
    class PopulationKnowledge_ABC;
    class ObjectKnowledge_ABC;
    class KnowledgeGroup_ABC;
    class Ghost_ABC;
    class MaterialCompositionType;
    class RoofShapeType;
}

namespace gui
{
// =============================================================================
/** @class  BaseDisplayer
    @brief  Base displayer
*/
// Created: AGE 2006-02-09
// =============================================================================
class BaseDisplayer : public kernel::Displayer_ABC
                    , public tools::Caller< bool >
                    , public tools::Caller< kernel::Duration >
                    , public tools::Caller< kernel::Population_ABC>
                    , public tools::Caller< kernel::Inhabitant_ABC>
                    , public tools::Caller< kernel::PopulationConcentration_ABC >
                    , public tools::Caller< kernel::Agent_ABC >
                    , public tools::Caller< kernel::Automat_ABC >
                    , public tools::Caller< kernel::Object_ABC >
                    , public tools::Caller< kernel::UrbanObject_ABC >
                    , public tools::Caller< kernel::Formation_ABC >
                    , public tools::Caller< kernel::Team_ABC >
                    , public tools::Caller< kernel::Entity_ABC >
                    , public tools::Caller< kernel::DotationType >
                    , public tools::Caller< kernel::EquipmentType >
                    , public tools::Caller< kernel::NBCAgent >
                    , public tools::Caller< kernel::FireClass >
                    , public tools::Caller< kernel::ComponentType >
                    , public tools::Caller< kernel::BreakdownType >
                    , public tools::Caller< kernel::Mission >
                    , public tools::Caller< kernel::AgentType >
                    , public tools::Caller< kernel::AutomatType >
                    , public tools::Caller< kernel::PopulationType >
                    , public tools::Caller< kernel::InhabitantType >
                    , public tools::Caller< kernel::ObjectType >
                    , public tools::Caller< kernel::KnowledgeGroupType >
                    , public tools::Caller< kernel::HierarchyLevel_ABC >
                    , public tools::Caller< kernel::Karma >
                    , public tools::Caller< kernel::AgentKnowledge_ABC >
                    , public tools::Caller< kernel::PopulationKnowledge_ABC >
                    , public tools::Caller< kernel::ObjectKnowledge_ABC >
                    , public tools::Caller< kernel::KnowledgeGroup_ABC >
                    , public tools::Caller< kernel::Ghost_ABC >
                    , public tools::Caller< kernel::MaterialCompositionType >
                    , public tools::Caller< kernel::RoofShapeType >
{
public:
    //! @name Constructors/Destructor
    //@{
             BaseDisplayer();
    virtual ~BaseDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Clear();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Call( const bool& value );
    virtual void Call( const kernel::Duration& value );
    virtual void Call( const kernel::Agent_ABC& value );
    virtual void Call( const kernel::Automat_ABC& value );
    virtual void Call( const kernel::Population_ABC& value );
    virtual void Call( const kernel::Inhabitant_ABC& value );
    virtual void Call( const kernel::PopulationConcentration_ABC& value );
    virtual void Call( const kernel::Object_ABC& value );
    virtual void Call( const kernel::UrbanObject_ABC& value );
    virtual void Call( const kernel::Team_ABC& value );
    virtual void Call( const kernel::Entity_ABC& value );
    virtual void Call( const kernel::Formation_ABC& value );
    virtual void Call( const kernel::DotationType& value );
    virtual void Call( const kernel::EquipmentType& value );
    virtual void Call( const kernel::NBCAgent& value );
    virtual void Call( const kernel::FireClass& value );
    virtual void Call( const kernel::ComponentType& value );
    virtual void Call( const kernel::BreakdownType& value );
    virtual void Call( const kernel::Mission& value );
    virtual void Call( const kernel::AgentType& value );
    virtual void Call( const kernel::AutomatType& value );
    virtual void Call( const kernel::InhabitantType& value );
    virtual void Call( const kernel::PopulationType& value );
    virtual void Call( const kernel::ObjectType& value );
    virtual void Call( const kernel::KnowledgeGroupType& value );
    virtual void Call( const kernel::HierarchyLevel_ABC& value );
    virtual void Call( const kernel::Karma& value );
    virtual void Call( const kernel::AgentKnowledge_ABC& value );
    virtual void Call( const kernel::PopulationKnowledge_ABC& value );
    virtual void Call( const kernel::ObjectKnowledge_ABC& value );
    virtual void Call( const kernel::KnowledgeGroup_ABC& value );
    virtual void Call( const kernel::Ghost_ABC& value );
    virtual void Call( const kernel::MaterialCompositionType& value );
    virtual void Call( const kernel::RoofShapeType& value );
    //@}
};

}

#endif // __BaseDisplayer_h_
