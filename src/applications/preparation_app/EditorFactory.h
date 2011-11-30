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
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/SubTypes.h"
#include "tools/Resolver.h"
#include "preparation/EnumTypes.h"

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class KnowledgeGroupType;
    class Moveable_ABC;
    class NBCAgent;
    class LogisticLevel;
}

class Model;
class StaticModel;
class DotationsItem;
class PopulationRepartition;

// =============================================================================
/** @class  EditorFactory
    @brief  EditorFactory
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
                    , public tools::Caller< kernel::LogisticLevel** >
                    , public tools::Caller< kernel::EntityLogisticLevel* >
                    , public tools::Caller< Enum_PopulationAttitude* >
                    , public tools::Caller< Enum_DemolitionTargetType* >
                    , public tools::Caller< Enum_NbcState* >
                    , public tools::Caller< DotationsItem** >
                    , public tools::Caller< kernel::Moveable_ABC** >
                    , public tools::Caller< PopulationRepartition** >
{
public:
    //! @name Constructors/Destructor
    //@{
             EditorFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
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
    virtual void Call( Enum_DemolitionTargetType* const& value );
    virtual void Call( Enum_NbcState* const& value );
    virtual void Call( DotationsItem** const& value );
    virtual void Call( kernel::LogisticLevel** const& value );
    virtual void Call( kernel::EntityLogisticLevel* const& value );
    virtual void Call( kernel::Moveable_ABC** const& value );
    virtual void Call( PopulationRepartition** const& value );
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
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QDialog* modalDialog_;
    //@}
};

#endif // __EditorFactory_h_
