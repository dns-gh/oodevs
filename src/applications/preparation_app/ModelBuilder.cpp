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
#include "clients_gui/Drawing.h"
#include "clients_gui/UrbanObject.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_gui/DictionaryUpdated.h"
#include "preparation/Agent.h"
#include "preparation/AgentsModel.h"
#include "preparation/Automat.h"
#include "preparation/AutomatPositions.h"
#include "preparation/EntityCommunications.h"
#include "preparation/Formation.h"
#include "preparation/FormationModel.h"
#include "preparation/Ghost.h"
#include "preparation/KnowledgeGroup.h"
#include "preparation/KnowledgeGroupsModel.h"
#include "preparation/LimitsModel.h"
#include "preparation/Model.h"
#include "preparation/Object.h"
#include "preparation/Population.h"
#include "preparation/TacticalLine_ABC.h"
#include "preparation/Team.h"
#include "preparation/TeamsModel.h"
#include "preparation/UrbanModel.h"
#include "tools/GeneralConfig.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

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
ModelBuilder::ModelBuilder( kernel::Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
    , selectedTeam_( controllers )
    , selectedGroup_( controllers )
    , selectedAgent_( controllers )
    , selectedAutomat_( controllers )
    , selectedFormation_( controllers )
    , selectedGhost_( controllers )
    , selectedObject_( controllers )
    , selectedUrbanObject_( controllers )
    , toDelete_( controllers )
    , confirmation_( new ConfirmationBox( tr( "Confirmation" ), boost::bind( &ModelBuilder::OnConfirmDeletion, this, _1 ) ) )
    , property_( tr( "Info/Name" ) )
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
        model_.formations_->Create( *selectedTeam_.ConstCast(), static_cast< E_NatureLevel >( levelId ) );
    else if( selectedFormation_ )
        model_.formations_->Create( *selectedFormation_.ConstCast(), static_cast< E_NatureLevel >( levelId ) );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreate
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreate()
{
    model_.teams_->CreateTeam();
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnCreateCommunication
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void ModelBuilder::OnCreateCommunication()
{
    if( selectedTeam_ ) // LTO
        model_.knowledgeGroups_->Create( *selectedTeam_.ConstCast() );
    else if( selectedGroup_ ) // LTO
        model_.knowledgeGroups_->CreateSubKnowledgeGroup( *selectedGroup_.ConstCast() ); // LTO
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CanCreateLine
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
bool ModelBuilder::CanCreateLine() const
{
    return selectedFormation_ || selectedAutomat_ || selectedAgent_;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CreateLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void ModelBuilder::CreateLimit( const T_PointVector& points )
{
    kernel::Entity_ABC* element = 0;
    if( selectedFormation_ )
        element = selectedFormation_.ConstCast();
    else if( selectedAutomat_ )
        element = selectedAutomat_.ConstCast();
    else if( selectedAgent_ )
        element = selectedAgent_.ConstCast();
    if( element )
        model_.limits_->CreateLimit( points, *element );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CreateLima
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void ModelBuilder::CreateLima( const T_PointVector& points )
{
    kernel::Entity_ABC* element = 0;
    if( selectedFormation_ )
        element = selectedFormation_.ConstCast();
    else if( selectedAutomat_ )
        element = selectedAutomat_.ConstCast();
    else if( selectedAgent_ )
        element = selectedAgent_.ConstCast();
    if( element )
        model_.limits_->CreateLima( points, *element );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CreateCityOrDistrict
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void ModelBuilder::CreateCityOrDistrict( kernel::Entity_ABC* parent )
{
    model_.urban_->CreateCityOrDistrict( parent );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::DeleteBlocks
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
void ModelBuilder::DeleteBlocks( const std::vector< const kernel::UrbanObject_ABC* >& urbanObjects )
{
    model_.urban_->DeleteBlocks( urbanObjects );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::NotifyContextMenu
// Created: SBO 2006-11-28
// -----------------------------------------------------------------------------
void ModelBuilder::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    // TODO gérer suivant les modes
    if( entity.GetTypeName() == kernel::UrbanObject_ABC::typeName_ )
        return;
    if( &entity != &model_.teams_->GetNoSideTeam() )
    {
        toDelete_ = &entity;
        menu.InsertItem( "Command", tr( "Delete" ), this, SLOT( OnDelete() ), false, 5 );
    }
}

namespace
{
    bool HasHierarchy( const kernel::Entity_ABC& entity )
    {
        const kernel::Hierarchies* hierarchy = entity.Retrieve< kernel::TacticalHierarchies >();
        if( !hierarchy || !hierarchy->CreateSubordinateIterator().HasMoreElements() )
        {
            hierarchy = entity.Retrieve< kernel::CommunicationHierarchies >();
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
        DeleteEntity( *toDelete_ );
}

namespace
{
    bool IsLastKnowledgeGroup( const kernel::Entity_ABC& entity, const Model& model )
    {
        const kernel::KnowledgeGroup_ABC* knowledgeGroup = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity );
        if( !knowledgeGroup )
            return false;
        const kernel::CommunicationHierarchies* hierarchies = knowledgeGroup->Retrieve< kernel::CommunicationHierarchies >();
        if( !hierarchies )
            return false;
        const kernel::Entity_ABC* superior = hierarchies->GetSuperior();
        if( !superior )
            return false;
        if( dynamic_cast< const kernel::KnowledgeGroup_ABC* >( superior ) )
            return false;
        unsigned int count = 0u;
        tools::Iterator< const kernel::KnowledgeGroup_ABC& > it = model.knowledgeGroups_->CreateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::KnowledgeGroup_ABC& group = it.NextElement();
            if( const kernel::CommunicationHierarchies* itHierarchies = group.Retrieve< kernel::CommunicationHierarchies >() )
                if( const kernel::Entity_ABC* itSuperior = itHierarchies->GetSuperior() )
                    if( itSuperior->GetId() == superior->GetId() )
                        ++count;
        }
        return count == 1;
    }
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::DeleteEntity
// Created: LGY 2011-11-28
// -----------------------------------------------------------------------------
void ModelBuilder::DeleteEntity( const kernel::Entity_ABC& entity )
{
    if( &entity == &model_.teams_->GetNoSideTeam() )
        return;
    toDelete_ = &entity;
    if( IsLastKnowledgeGroup( *toDelete_, model_ ) )
    {
        QMessageBox::warning( 0, tr( "Warning" ), tr( "Unable to delete the knowledge group. At least one knowledge group must exists for each side." ) );
        return;
    }
    if( HasHierarchy( entity ) )
        confirmation_->setText( tr( "Delete '%1' and all its subordinates?" ).arg( entity.GetName() ) );
    else
        confirmation_->setText( tr( "Delete '%1'?" ).arg( entity.GetName() ) );
    confirmation_->adjustSize();
    confirmation_->show();
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnConfirmDeletion
// Created: SBO 2010-03-22
// -----------------------------------------------------------------------------
void ModelBuilder::OnConfirmDeletion( int result )
{
    if( result == QMessageBox::Yes && toDelete_ )
        Delete( *toDelete_ );
}

namespace
{
    void EntityToDelete( std::vector< const kernel::Entity_ABC* >& vect )
    {
        for( std::vector< const kernel::Entity_ABC* >::iterator it = vect.begin(); it != vect.end(); ++it )
        {
            if( const kernel::TacticalHierarchies* hierarchies2 = ( *it )->Retrieve< kernel::TacticalHierarchies >() )
            {
                tools::Iterator< const kernel::Entity_ABC& > it2 = hierarchies2->CreateSubordinateIterator();
                while( it2.HasMoreElements() )
                {
                    std::vector< const kernel::Entity_ABC* >::iterator it3 = std::find( vect.begin(), vect.end(), &it2.NextElement() );
                    if( it3 != vect.end() )
                        vect.erase( it3 );
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Delete
// Created: AGE 2006-11-28
// -----------------------------------------------------------------------------
void ModelBuilder::Delete( const kernel::Entity_ABC& entity )
{
    if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        tools::Iterator< const kernel::Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
        while( it.HasMoreElements() )
            Delete( it.NextElement() );
    }
    if( const kernel::CommunicationHierarchies* hierarchies = entity.Retrieve< kernel::CommunicationHierarchies >() )
    {
        tools::Iterator< const kernel::Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
        std::vector< const kernel::Entity_ABC* > subordinate;
        while( it.HasMoreElements() )
        {
            const kernel::Entity_ABC& subEntity = it.NextElement();
            if( dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &subEntity ) )
                Delete( subEntity );
            else
                subordinate.push_back( &subEntity );
        }
        EntityToDelete( subordinate );   // $$$$ _RC_ SLG 2010-11-12: supprime les automates d'automates afin d'éviter un crash de la sim
        const kernel::KnowledgeGroup_ABC* kg = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity );
        kernel::KnowledgeGroup_ABC* kgTarget = 0;
        if( kg )
        {
            const kernel::Entity_ABC& top = kg->Get< kernel::CommunicationHierarchies >().GetTop();
            const kernel::CommunicationHierarchies& teamHierarchy = top.Get< kernel::CommunicationHierarchies >();
            tools::Iterator< const kernel::Entity_ABC& > it = teamHierarchy.CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                kgTarget = dynamic_cast< kernel::KnowledgeGroup_ABC* >( const_cast< kernel::Entity_ABC* >( &it.NextElement() ) );
                if( kgTarget && kgTarget != &entity )
                {
                    const kernel::CommunicationHierarchies& comHierarchy = kg->Get< kernel::CommunicationHierarchies >();
                    tools::Iterator< const kernel::Entity_ABC& > it = comHierarchy.CreateSubordinateIterator();
                    while( it.HasMoreElements() )
                    {
                        kernel::Entity_ABC& sub = const_cast< kernel::Entity_ABC& >( it.NextElement() );
                        EntityCommunications* subComHierarchy = dynamic_cast< EntityCommunications* >( &sub.Get< kernel::CommunicationHierarchies >() );
                        if( subComHierarchy)
                            subComHierarchy->ChangeSuperior( *kgTarget );
                    }
                    break;
                }
            }
        }
        if( !kgTarget )
            for( std::vector< const kernel::Entity_ABC* >::iterator it = subordinate.begin(); it != subordinate.end(); ++it )
                Delete( **it );
    }
    delete &entity;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::ReplaceAutomat
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
kernel::Automat_ABC* ModelBuilder::ReplaceAutomat( kernel::Entity_ABC& original, const kernel::AutomatType& type )
{
    const kernel::Positions& position = original.Get< kernel::Positions >();
    kernel::Automat_ABC* result = model_.agents_->CreateAutomatInsteadOf( original, type, position.GetPosition( false ) );
    if( result )
        Delete( original );
    return result;
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
    selectedGhost_ = 0;
    selectedObject_ = 0;
    selectedUrbanObject_ = 0;
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
void ModelBuilder::Select( const kernel::Team_ABC& element )
{
    ClearSelection();
    selectedTeam_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::KnowledgeGroup_ABC& element )
{
    ClearSelection();
    selectedGroup_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::Agent_ABC& element )
{
    ClearSelection();
    selectedAgent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::Formation_ABC& element )
{
    ClearSelection();
    selectedFormation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: LGY 2012-08-28
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::UrbanObject_ABC& element )
{
    ClearSelection();
    selectedUrbanObject_ = &element;
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

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: ABR 2011-10-20
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::Ghost_ABC& element )
{
    ClearSelection();
    selectedGhost_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::Select
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void ModelBuilder::Select( const kernel::Object_ABC& element )
{
    ClearSelection();
    selectedObject_ = &element;
}

namespace
{
    template< typename Concrete, typename T >
    void Rename( T* entity, const QString& text, kernel::Controllers& controllers, const QString& property )
    {
        if( Concrete* concrete = dynamic_cast< Concrete* >( entity ) )
        {
            concrete->Rename( text );
            controllers.controller_.Update( gui::DictionaryUpdated( *concrete, property ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::OnRename
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
void ModelBuilder::OnRename( kernel::Entity_ABC& entity, const QString& newName )
{
    Rename< Team >( &entity, newName, controllers_, property_ );
    Rename< Agent >( &entity, newName, controllers_, property_ );
    Rename< Automat >( &entity, newName, controllers_, property_ );
    Rename< Formation >( &entity, newName, controllers_, property_ );
    Rename< KnowledgeGroup >( &entity, newName, controllers_, property_ );
    Rename< Ghost >( &entity, newName, controllers_, property_ );
    Rename< Object >( &entity, newName, controllers_, property_ );
    Rename< gui::UrbanObject >( &entity, newName, controllers_, property_ );
    Rename< Population >( &entity, newName, controllers_, property_ );
    Rename< gui::Drawing >( &entity, newName, controllers_, property_ );
    Rename< TacticalLine_ABC >( &entity, newName, controllers_, property_ );
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CreateCommunication
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void ModelBuilder::CreateCommunication()
{
    OnCreateCommunication();
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CreateTeam
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void ModelBuilder::CreateTeam()
{
    OnCreate();
}

// -----------------------------------------------------------------------------
// Name: ModelBuilder::CreateFormation
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void ModelBuilder::CreateFormation( int level )
{
    OnCreateFormation( level );
}

