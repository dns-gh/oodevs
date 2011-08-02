// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ModelBuilder.h"
#include "moc_ModelBuilder.cpp"
#include "icons.h"
#include "preparation/Model.h"
#include "preparation/TeamsModel.h"
#include "preparation/AgentsModel.h"
#include "preparation/FormationModel.h"
#include "preparation/KnowledgeGroupsModel.h"
#include "preparation/LimitsModel.h"
#include "preparation/Team.h"
#include "preparation/Formation.h"
#include "preparation/KnowledgeGroup.h"
#include "preparation/Automat.h"
#include "preparation/Agent.h"
#include "preparation/Object.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/Level.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "tools/GeneralConfig.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace kernel;

namespace
{
    class ConfirmationBox : public QMessageBox
    {
    private:
        typedef boost::function1< void, int > T_Callback;
    public:
        ConfirmationBox( const QString& title, T_Callback callback )
            : QMessageBox( title, "", QMessageBox::Warning, QMessageBox::Yes, QMessageBox::No | QMessageBox::Default, Qt::NoButton )
            , callback_( callback )
        {
            setIcon( QMessageBox::Information );
            hide();
        }

        virtual void done( int result )
        {
            callback_( result );
            hide();
        }
    private:
        T_Callback callback_;
    };
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder constructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
ModelBuilder::ModelBuilder( Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
    , selectedTeam_( controllers )
    , selectedGroup_( controllers )
    , selectedAgent_( controllers )
    , selectedAutomat_( controllers )
    , selectedFormation_( controllers )
    , toDelete_( controllers )
    , confirmation_( new ConfirmationBox( tr( "Confirmation" ), boost::bind( &ModelBuilder::OnConfirmDeletion, this, _1 ) ) )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder destructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
ModelBuilder::~ModelBuilder()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreateFormation
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreateFormation( int levelId )
{
    if( selectedTeam_ )
        model_.formations_.Create( *selectedTeam_.ConstCast(), levelId );
    else if( selectedFormation_ )
        model_.formations_.Create( *selectedFormation_.ConstCast(), levelId );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreate
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreate()
{
    model_.teams_.CreateTeam();
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreateCommunication
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreateCommunication()
{
    if( selectedTeam_ ) // LTO
        model_.knowledgeGroups_.Create( *selectedTeam_.ConstCast() );
    else if( selectedGroup_ ) // LTO
        model_.knowledgeGroups_.CreateSubKnowledgeGroup( *selectedGroup_.ConstCast() ); // LTO
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CanCreateLine
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
bool ModelBuilder::CanCreateLine() const
{
    return selectedFormation_ || selectedAutomat_;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CreateLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void ModelBuilder::CreateLimit( const T_PointVector& points )
{
    const Entity_ABC* element = selectedFormation_ ? (const Entity_ABC*)selectedFormation_ : (const Entity_ABC*)selectedAutomat_;
    if( element )
        model_.limits_.CreateLimit( points, *const_cast< kernel::Entity_ABC* >( element ) );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CreateLima
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void ModelBuilder::CreateLima( const T_PointVector& points )
{
    const Entity_ABC* element = selectedFormation_ ? (const Entity_ABC*)selectedFormation_ : (const Entity_ABC*)selectedAutomat_;
    if( element )
        model_.limits_.CreateLima( points, *const_cast< kernel::Entity_ABC* >( element ) );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-11-28
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    toDelete_ = &entity;
    menu.InsertItem( "Command", tr( "Delete" ), this, SLOT( OnDelete() ) );
}

namespace
{
    bool HasHierarchy( const Entity_ABC* entity )
    {
        const kernel::Hierarchies* hierarchy = entity->Retrieve< kernel::TacticalHierarchies >();
        if( !hierarchy || !hierarchy->CreateSubordinateIterator().HasMoreElements() )
        {
            hierarchy = entity->Retrieve< kernel::CommunicationHierarchies >();
            if( !hierarchy || !hierarchy->CreateSubordinateIterator().HasMoreElements() )
                return false;
        }
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnDelete
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::OnDelete()
{
    if( toDelete_ )
    {
        if( HasHierarchy( toDelete_ ) )
            confirmation_->setText( tr( "Delete '%1' and all its subordinates?" ).arg( toDelete_->GetName() ) );
        else
            confirmation_->setText( tr( "Delete '%1'?" ).arg( toDelete_->GetName() ) );
        confirmation_->adjustSize();
        confirmation_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnDelete
// Created: SBO 2010-03-22
// -----------------------------------------------------------------------------
void ModelBuilder::OnConfirmDeletion( int result )
{
    if( result == QMessageBox::Yes && toDelete_ )
        DeleteEntity( *toDelete_ );
}

namespace
{
    void EntityToDelete( std::vector< const Entity_ABC* >& vect )
    {
        for( std::vector< const Entity_ABC* >::iterator it = vect.begin(); it != vect.end(); ++it )
        {
            if( const kernel::TacticalHierarchies* hierarchies2 = ( *it )->Retrieve< kernel::TacticalHierarchies >() )
            {
                tools::Iterator< const Entity_ABC& > it2 = hierarchies2->CreateSubordinateIterator();
                while( it2.HasMoreElements() )
                {
                    std::vector< const Entity_ABC* >::iterator it3 = std::find( vect.begin(), vect.end(), &it2.NextElement() );
                    if( it3 != vect.end() )
                        vect.erase( it3 );
                }
            }
        }
    }
}
// -----------------------------------------------------------------------------
// Name: ModelBuilder::DeleteEntity
// Created: AGE 2006-11-28
// -----------------------------------------------------------------------------
void ModelBuilder::DeleteEntity( const Entity_ABC& entity )
{
    if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        tools::Iterator< const Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
        while( it.HasMoreElements() )
            DeleteEntity( it.NextElement() );
    }
    if( const kernel::CommunicationHierarchies* hierarchies = entity.Retrieve< kernel::CommunicationHierarchies >() )
    {
        tools::Iterator< const Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
        std::vector< const Entity_ABC* > subordinate;
        while( it.HasMoreElements() )
            subordinate.push_back( &it.NextElement() );
        EntityToDelete( subordinate );   // $$$$ _RC_ SLG 2010-11-12: supprime les automates d'automates afin d'éviter un crash de la sim
        for( std::vector< const Entity_ABC* >::iterator it = subordinate.begin(); it != subordinate.end(); ++it )
            DeleteEntity( **it );
    }
    delete &entity;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::ClearSelection
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void ModelBuilder::ClearSelection()
{
    selectedTeam_ = 0;
    selectedGroup_ = 0;
    selectedAgent_ = 0;
    selectedAutomat_ = 0;
    selectedFormation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::BeforeSelection
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::BeforeSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::AfterSelection
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Team_ABC& element )
{
    ClearSelection();
    selectedTeam_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const KnowledgeGroup_ABC& element )
{
    ClearSelection();
    selectedGroup_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Agent_ABC& element )
{
    ClearSelection();
    selectedAgent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const Formation_ABC& element )
{
    ClearSelection();
    selectedFormation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: AGE 2006-10-10
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::Automat_ABC& element )
{
    ClearSelection();
    selectedAutomat_ = &element;
}

namespace
{
    template< typename Concrete, typename T >
    void Rename( T& entity, const QString& text )
    {
        if( Concrete* concrete = dynamic_cast< Concrete* >( entity.ConstCast() ) )
            concrete->Rename( text );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnRename
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void ModelBuilder::OnRename( Q3ListViewItem*, int, const QString& text )
{
    if( selectedTeam_ )
        Rename< Team >( selectedTeam_, text );
    else if( selectedAgent_ )
        Rename< Agent >( selectedAgent_, text );
    else if( selectedAutomat_ )
        Rename< Automat >( selectedAutomat_, text );
    else if( selectedFormation_ )
        Rename< Formation >( selectedFormation_, text );
    else if( selectedGroup_ )
        Rename< KnowledgeGroup >( selectedGroup_, text );
}
