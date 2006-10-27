// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "EditorFactory.h"
#include "LogisticSuperiorEditor.h"
#include "preparation/LogisticSuperior.h"
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"
#include "preparation/StaticModel.h"
#include "preparation/KnowledgeGroupTypes.h"
#include "preparation/KnowledgeGroupType.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/ValueEditor.h"

// -----------------------------------------------------------------------------
// Name: EditorFactory constructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
EditorFactory::EditorFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel )
    : gui::EditorFactory()
    , controllers_( controllers )
    , model_( model )
    , staticModel_( staticModel )
    , selected_( controllers )
{
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: EditorFactory destructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
EditorFactory::~EditorFactory()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::BuildLogisticEditor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
template< typename T >
void EditorFactory::BuildLogisticEditor( T& value )
{
    if( !selected_ )
        return;
    LogisticSuperiorEditor< T >* editor = new LogisticSuperiorEditor< T >( parent_, controllers_, model_.agents_, *selected_ );
    editor->SetCurrentItem( value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( TC2* const& value )
{
    BuildLogisticEditor( *value );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( MaintenanceSuperior* const& value )
{
    BuildLogisticEditor( *value );
}
    
// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( MedicalSuperior* const& value )
{
    BuildLogisticEditor( *value );
}
    
// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void EditorFactory::Call( SupplySuperior* const& value )
{
    BuildLogisticEditor( *value );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::NotifySelected
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void EditorFactory::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
}

namespace
{
    template< typename Entity, typename Resolver >
    class SimpleResolverEditor : public gui::ValuedComboBox< const Entity* >
                               , public kernel::ValueEditor< Entity* >
    {
    public:
        SimpleResolverEditor( QWidget* parent, const Resolver& resolver ) 
            : gui::ValuedComboBox< const Entity* >( parent )
        {
            kernel::Iterator< const Entity& > it = resolver.CreateIterator();
            while( it.HasMoreElements() )
            {
                const Entity& entity = it.NextElement();
                AddItem( entity.GetName(), &entity );
            }
        }
        virtual ~SimpleResolverEditor() {}

        virtual Entity* GetValue()
        {
            return const_cast< Entity* >( gui::ValuedComboBox< const Entity* >::GetValue() );
        }

    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void EditorFactory::Call( KnowledgeGroupType** const& value )
{
    SimpleResolverEditor< KnowledgeGroupType, KnowledgeGroupTypes >* editor = new SimpleResolverEditor< KnowledgeGroupType, KnowledgeGroupTypes >( parent_, staticModel_.knowledgeGroupTypes_ );
    editor->SetCurrentItem( *value );
    result_ = editor;
}
