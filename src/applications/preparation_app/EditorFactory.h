// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EditorFactory_h_
#define __EditorFactory_h_

#include "clients_gui/EditorFactory.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/SubTypes.h"
#include "preparation/EnumTypes.h"
#include <tools/Resolver.h>
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class InfrastructureType;
    class KnowledgeGroupType;
    class MaterialCompositionType;
    class Moveable_ABC;
    class NBCAgent;
    class RoofShapeType;
    class UrbanTemplateType;
    struct UrbanBlockColor;
}

namespace tools
{
    class GeneralConfig;
}

class Model;
class StaticModel;
class PopulationRepartition;

// =============================================================================
/** @class  EditorFactory
    @brief  Editor factory
*/
// Created: SBO 2006-10-25
// =============================================================================
class EditorFactory : public gui::EditorFactory
                    , public tools::Observer_ABC
                    , public tools::SelectionObserver< kernel::Entity_ABC >
                    , public tools::Caller< kernel::LogisticBaseSuperior* >
                    , public tools::Caller< kernel::KnowledgeGroupType** >
                    , public tools::Caller< kernel::TeamKarma* >
                    , public tools::Caller< kernel::NBCAgent** >
                    , public tools::Caller< std::vector< kernel::NBCAgent* >* >
                    , public tools::Caller< Enum_PopulationAttitude* >
                    , public tools::Caller< Enum_NbcState* >
                    , public tools::Caller< kernel::Moveable_ABC** >
                    , public tools::Caller< PopulationRepartition** >
                    , public tools::Caller< kernel::InfrastructureType** >
                    , public tools::Caller< kernel::UrbanTemplateType** >
                    , public tools::Caller< kernel::MaterialCompositionType** >
                    , public tools::Caller< kernel::RoofShapeType** >
                    , public tools::Caller< kernel::UrbanBlockColor* >
                    , public tools::Caller< kernel::CriticalIntelligenceType* >
                    , public tools::Caller< kernel::DisasterDirectory* >
{
public:
    //! @name Constructors/Destructor
    //@{
             EditorFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel,
                            const tools::GeneralConfig& config );
    virtual ~EditorFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Call( kernel::LogisticBaseSuperior* const& value );
    virtual void Call( kernel::KnowledgeGroupType** const& value );
    virtual void Call( kernel::TeamKarma* const& value );
    virtual void Call( kernel::NBCAgent** const& value );
    virtual void Call( std::vector< kernel::NBCAgent* >* const& value );
    virtual void Call( Enum_PopulationAttitude* const& value );
    virtual void Call( Enum_NbcState* const& value );
    virtual void Call( kernel::Moveable_ABC** const& value );
    virtual void Call( PopulationRepartition** const& value );
    virtual void Call( kernel::InfrastructureType** const& value );
    virtual void Call( kernel::UrbanTemplateType** const& value );
    virtual void Call( kernel::MaterialCompositionType** const& value );
    virtual void Call( kernel::RoofShapeType** const& value );
    virtual void Call( kernel::UrbanBlockColor* const& value );
    virtual void Call( kernel::CriticalIntelligenceType* const& value );
    virtual void Call( kernel::DisasterDirectory* const& value );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& staticModel_;
    const tools::GeneralConfig& config_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __EditorFactory_h_
