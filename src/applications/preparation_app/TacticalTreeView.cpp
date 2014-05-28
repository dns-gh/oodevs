// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TacticalTreeView.h"
#include "moc_TacticalTreeView.cpp"
#include "ChangeAutomatTypeDialog.h"

#include "clients_gui/AutomatDecisions.h"
#include "clients_gui/ModelObserver_ABC.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_gui/Tools.h"

#include "clients_kernel/ActionController.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_gui/EntityType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Positions.h"

#include "preparation/AgentsModel.h"
#include "preparation/EntityCommunications.h"
#include "preparation/Formation.h"
#include "preparation/Model.h"
#include "preparation/TacticalHierarchies.h"

// -----------------------------------------------------------------------------
// Name: TacticalTreeView constructor
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
TacticalTreeView::TacticalTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, Model& model, const kernel::AgentTypes& agentTypes, QWidget* parent /* = 0 */ )
    : gui::TacticalTreeView( objectName, controllers, profile, modelObserver, symbols, parent )
    , model_( model )
    , agentTypes_( agentTypes )
    , contextMenuEntity_( controllers )
    , changeSuperiorDialog_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView destructor
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
TacticalTreeView::~TacticalTreeView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::ContextMenuRequested
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::ContextMenuRequested( const QPoint& where )
{
    if( !isVisible() )
        return;
    modelObserver_.ClearSelection();
    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    menu->insertItem( tr( "Create side" ), this, SLOT( OnCreateTeam() ) );
    menu->exec( where );
}

namespace
{
    kernel::Entity_ABC& GetFirstCommunicationChild( const kernel::Entity_ABC& entity )
    {
        tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::CommunicationHierarchies >().CreateSubordinateIterator();
        if( it.HasMoreElements() )
            return const_cast< kernel::Entity_ABC& >( it.NextElement() );
        throw MASA_EXCEPTION( "No communication child found" );
    }

    void UpdateCommunicationHierarchies( kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior )
    {
        const kernel::Entity_ABC& tacticalTop = superior.Get< kernel::TacticalHierarchies >().GetTop();
        kernel::CommunicationHierarchies* com = entity.Retrieve< kernel::CommunicationHierarchies >();

        if( com && &com->GetTop() != &tacticalTop )
            static_cast< ::EntityCommunications* >( com )->ChangeSuperior( GetFirstCommunicationChild( tacticalTop ) );

        tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
        while( it.HasMoreElements() )
            UpdateCommunicationHierarchies( const_cast< kernel::Entity_ABC& >( it.NextElement() ), superior );
    }

    void ChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) // $$$$ SBO 2006-09-28: cast-machine
    {
        kernel::Entity_ABC& ent = const_cast< kernel::Entity_ABC& >( entity );
        if( kernel::TacticalHierarchies* hierarchies = ent.Retrieve< kernel::TacticalHierarchies >() )
        {
            UpdateCommunicationHierarchies( ent, superior );
            static_cast< ::TacticalHierarchies* >( hierarchies )->ChangeSuperior( const_cast< kernel::Entity_ABC& >( superior ) );
        }
    }
}

namespace
{
    bool IsSubordinateOf( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior )
    {
        if( const kernel::TacticalHierarchies* pTactical = superior.Retrieve< kernel::TacticalHierarchies >() )
            return pTactical->IsSubordinateOf( entity );
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::CanChangeSuperior
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
bool TacticalTreeView::CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const
{
    if( !superior.Retrieve< kernel::TacticalHierarchies >() )
        return false;
    if( dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        return dynamic_cast< const kernel::Automat_ABC* >( &superior ) != 0;
    if( dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        return dynamic_cast< const kernel::Formation_ABC* >( &superior ) != 0;
    if( dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        return ( !IsSubordinateOf( entity, superior ) && dynamic_cast< const kernel::Formation_ABC* >( &superior ) != 0 ) || dynamic_cast< const kernel::Team_ABC* >( &superior ) != 0;
    if( const kernel::Ghost_ABC* ghost = dynamic_cast< const kernel::Ghost_ABC* >( &entity ) )
        return ( ghost->GetGhostType() == eGhostType_Automat && dynamic_cast< const kernel::Formation_ABC* >( &superior ) != 0 ) ||
               ( ghost->GetGhostType() == eGhostType_Agent && dynamic_cast< const kernel::Automat_ABC* >( &superior ) != 0 );
    if( dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity ) )
        return dynamic_cast< const kernel::Formation_ABC* >( &superior ) != 0;
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target )
{
    if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &target ) )
    {
        ChangeSuperior( item, target );
        kernel::CommunicationHierarchies* com = const_cast< kernel::Agent_ABC& >( item ).Retrieve< kernel::CommunicationHierarchies >();
        if( com )
            static_cast< ::EntityCommunications* >( com )->ChangeSuperior( const_cast< kernel::Entity_ABC& >( target ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target)
{
    if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &target ) )
        ChangeSuperior( item, target );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target )
{
    if( dynamic_cast< const kernel::Formation_ABC* >( &target ) || dynamic_cast< const kernel::Team_ABC* >( &target ) )
        ChangeSuperior( item, target );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Ghost_ABC& item, const kernel::Entity_ABC& target )
{
    const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &target );
    const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &target );
    if( ( formation && item.GetGhostType() == eGhostType_Automat )
     || ( automat && item.GetGhostType() == eGhostType_Agent ) )
        ChangeSuperior( item, target );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target )
{
    if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &target ) )
    {
        tools::Iterator< const kernel::Entity_ABC& > children = item.Get< kernel::CommunicationHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
            ChangeSuperior( children.NextElement(), *formation );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::AgentType& item, kernel::Entity_ABC& target )
{
    const geometry::Point2f position = target.Get< kernel::Positions >().GetPosition();
    if( kernel::Entity_ABC* result = model_.agents_->CreateAgent( static_cast< kernel::Ghost_ABC& >( target ), item, position ) )
    {
        delete &target;
        setFocus();
        result->Select( controllers_.actions_ );
        kernel::GraphicalEntity_ABC::T_GraphicalEntities list;
        list.push_back( result );
        result->MultipleSelect( controllers_.actions_, list );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::AutomatType& item, kernel::Entity_ABC& target )
{
    const geometry::Point2f position = target.Get< kernel::Positions >().GetPosition();
    if( kernel::Entity_ABC* result = model_.agents_->CreateAutomatInsteadOf( target, item, position ) )
    {
        delete &target;
        setFocus();
        result->Select( controllers_.actions_ );
        kernel::GraphicalEntity_ABC::T_GraphicalEntities list;
        list.push_back( result );
        result->MultipleSelect( controllers_.actions_, list );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::OnCreateTeam
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::OnCreateTeam()
{
    modelObserver_.CreateTeam();
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::AddCommonMenu
// Created: LGY 2014-05-13
// -----------------------------------------------------------------------------
void TacticalTreeView::AddCommonMenu( kernel::ContextMenu& menu, const kernel::Entity_ABC& entity )
{
    if( !IsActivated() )
        return;
    if( entity.Get< kernel::TacticalHierarchies >().GetSuperior() != 0 )
        menu.InsertItem( "Command", tr( "Change superior" ), this, SLOT( OnChangeSuperior() ), false, 1 );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyContextMenu
// Created: LGY 2014-05-13
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    contextMenuEntity_ = &agent;
    AddCommonMenu( menu, agent );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyContextMenu
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu )
{
    contextMenuEntity_ = &team;
    AddCommonMenu( menu, team );

    if( !isVisible() || !IsActivated() )
        return;
    AddFormationMenu( menu, eNatureLevel_xxxxx );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyContextMenu
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    contextMenuEntity_ = &formation;
    AddCommonMenu( menu, formation );

    if( !isVisible() || !IsActivated() )
        return;
    if( formation.GetLevel() > eNatureLevel_c )
        AddFormationMenu( menu, static_cast< E_NatureLevel >( formation.GetLevel() ) );

    kernel::ContextMenu* subMenu = menu.SubMenu( "Helpers", tr( "Change hierarchy level" ), false, 4 );
    for( int level = static_cast< int >( eNatureLevel_xxxxx ); level > 0; level-- )
        subMenu->insertItem( ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( level ), ENT_Tr::eToTr ).c_str(), this, SLOT( OnChangeLevel( int ) ), 0, level );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyContextMenu
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    contextMenuEntity_ = &automat;
    AddCommonMenu( menu, automat );

    if( !isVisible() || !IsActivated() )
        return;
    if( tools::IsEngaged( automat ) )
        menu.InsertItem( "Helpers", tr( "Disengage" ), this, SLOT( Disengage() ), false, 0 );
    else
        menu.InsertItem( "Helpers", tr( "Engage" ), this, SLOT( Engage() ), false, 0 );
    menu.InsertItem( "Command", tr( "Change automat type" ), this, SLOT( ChangeAutomatType() ), false, 0 );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyContextMenu
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyContextMenu( const kernel::Ghost_ABC& ghost, kernel::ContextMenu& menu )
{
    if( ghost.GetGhostType() != eGhostType_Automat || !isVisible() || !IsActivated() )
        return;
    contextMenuEntity_ = &ghost;
    AddCommonMenu( menu, ghost );
    menu.InsertItem( "Command", tr( "Replace by a new automat" ), this, SLOT( ChangeAutomatType() ), false, 6 );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::DoChangeSuperior
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior )
{
    kernel::Agent_ABC* agent = dynamic_cast< kernel::Agent_ABC* >( &entity );
    kernel::Automat_ABC* automat = dynamic_cast< kernel::Automat_ABC* >( &entity );
    kernel::Formation_ABC* formation = dynamic_cast< kernel::Formation_ABC* >( &entity );
    kernel::Ghost_ABC* ghost = dynamic_cast< kernel::Ghost_ABC* >( &entity );
    kernel::KnowledgeGroup_ABC* kg = dynamic_cast< kernel::KnowledgeGroup_ABC* >( &entity );
    if( agent )
        Drop( *agent, superior );
    else if( automat )
        Drop( *automat, superior );
    else if( formation )
        Drop( *formation, superior );
    else if( ghost )
        Drop( *ghost, superior );
    else if( kg )
        Drop( *kg, superior );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::keyPressEvent
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
void TacticalTreeView::keyPressEvent( QKeyEvent* event )
{
    const QModelIndex index = selectionModel()->currentIndex();
    if( event && event->key() == Qt::Key_Delete && index.isValid() )
    {
        if( kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index ) )
            modelObserver_.DeleteEntity( *entity );
    }
    else
        gui::TacticalTreeView::keyPressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::AddFormationMenu
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::AddFormationMenu( kernel::ContextMenu& menu, E_NatureLevel root )
{
    kernel::ContextMenu* subMenu = menu.SubMenu( "Command", tr( "Create formation" ), false, 3 );
    for( int level = static_cast< int >( root ); level > 0; level-- )
        subMenu->insertItem( ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( level ), ENT_Tr::eToTr ).c_str(), this, SLOT( OnCreateFormation( int ) ), 0, level );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::OnCreateFormation
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::OnCreateFormation( int level )
{
    modelObserver_.CreateFormation( level );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::OnChangeLevel
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::OnChangeLevel( int levelId )
{
    if( contextMenuEntity_ )
    {
        if( contextMenuEntity_->GetTypeName() != kernel::Formation_ABC::typeName_ )
            return;
        static_cast< Formation* >( contextMenuEntity_.ConstCast() )->SetLevel( static_cast< E_NatureLevel >( levelId ) );
        if( kernel::TacticalHierarchies* pTactical = contextMenuEntity_.ConstCast()->Retrieve< kernel::TacticalHierarchies >() )
        {
            pTactical->UpdateSymbolUpward();
            controllers_.controller_.Update( *pTactical );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::OnChangeSuperior
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::OnChangeSuperior()
{
    if( contextMenuEntity_ )
    {
        if( !changeSuperiorDialog_ )
            changeSuperiorDialog_ = new gui::ChangeSuperiorDialog( this, controllers_, *this, false );
        changeSuperiorDialog_->Show( *contextMenuEntity_.ConstCast() );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Engage
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::Engage()
{
    if( contextMenuEntity_ )
        if( gui::AutomatDecisions* decisions =
            static_cast< gui::AutomatDecisions* >( contextMenuEntity_.ConstCast()->Retrieve< gui::Decisions_ABC >() ) )
            decisions->SetEngaged( true );
    doItemsLayout();
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Disengage
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::Disengage()
{
    if( contextMenuEntity_ )
        if( gui::AutomatDecisions* decisions =
            static_cast< gui::AutomatDecisions* >( contextMenuEntity_.ConstCast()->Retrieve< gui::Decisions_ABC >() ) )
            decisions->SetEngaged( false );
    doItemsLayout();
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::ChangeAutomatType
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
void TacticalTreeView::ChangeAutomatType()
{
    std::string typeName = "";
    if( !contextMenuEntity_ )
        return;
    if( contextMenuEntity_->GetTypeName() == kernel::Automat_ABC::typeName_ )
        typeName = contextMenuEntity_->Get< gui::EntityType< kernel::AutomatType > >().GetType().GetName();
    else if( contextMenuEntity_->GetTypeName() == kernel::Ghost_ABC::typeName_ )
        typeName = static_cast< const kernel::Ghost_ABC& >( *contextMenuEntity_ ).GetType();
    else
        return;
    ChangeAutomatTypeDialog( this, controllers_, agentTypes_, modelObserver_, *contextMenuEntity_.ConstCast(), typeName );
}
