// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "TacticalListView.h"
#include "moc_TacticalListView.cpp"
#include "ChangeAutomatTypeDialog.h"
#include "ModelBuilder.h"
#include "PreparationProfile.h"
#include "Preparation/Agent.h"
#include "preparation/AgentsModel.h"
#include "preparation/AutomatDecisions.h"
#include "preparation/TacticalHierarchies.h"
#include "preparation/EntityCommunications.h"
#include "Preparation/Formation.h"
#include "Preparation/FormationModel.h"
#include "preparation/LogisticHierarchiesBase.h"
#include "preparation/LogisticBaseStates.h"
#include "preparation/Model.h"
#include "preparation/ProfileHierarchies_ABC.h"
#include "preparation/ProfileHierarchies.h"
#include "preparation/LogisticLevelAttritube.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/Positions.h"
#include "ENT/ENT_Tr_Gen.h"
#include "icons.h"

using namespace gui;
using namespace kernel;

namespace
{
    bool IsCommandPost( const kernel::Entity_ABC& entity )
    {
        if( const kernel::CommandPostAttributes_ABC* pAttributes = entity.Retrieve< kernel::CommandPostAttributes_ABC >() )
            return pAttributes->IsCommandPost();
        return false;
    }

    int ItemComparator( const ValuedListItem& item1, const ValuedListItem& item2, int /*col*/, bool /*ascending*/ )
    {
        const Entity_ABC* entity1 = item1.GetValue< const Entity_ABC >();
        if( !entity1 || IsCommandPost( *entity1 ) )
            return -1;
        const Entity_ABC* entity2 = item2.GetValue< const Entity_ABC >();
        if( !entity2 || IsCommandPost( *entity2 ) )
            return 1;
        return entity1->GetId() - entity2->GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, EntitySymbols& icons, Model& model,
                                    ModelBuilder& modelBuilder, const kernel::AgentTypes& agentTypes, const kernel::GlTools_ABC& tools )
    : HierarchyListView< kernel::TacticalHierarchies >( pParent, controllers, factory, PreparationProfile::GetProfile(), icons )
    , itemFactory_         ( factory )
    , agentTypes_          ( agentTypes )
    , model_               ( model )
    , modelBuilder_        ( modelBuilder )
    , tools_               ( tools )
    , lock_                ( MAKE_PIXMAP( lock ) )
    , changeSuperiorDialog_( 0 )
    , contextMenuEntity_   ( controllers )
{
    controllers_.Register( *this );
    addColumn( "HiddenPuce", 15 );
    setColumnAlignment( 1, Qt::AlignCenter );
    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), &modelBuilder_, SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
    SetComparator( &ItemComparator );
    controllers.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
TacticalListView::~TacticalListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::hideEvent
// Created: JSR 2011-11-09
// -----------------------------------------------------------------------------
void TacticalListView::hideEvent( QHideEvent* event )
{
    if( changeSuperiorDialog_ )
        changeSuperiorDialog_->hide();
    HierarchyListView< kernel::TacticalHierarchies >::hideEvent( event );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::viewportResizeEvent
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::viewportResizeEvent( QResizeEvent* e )
{
    Q3ScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void TacticalListView::setColumnWidth( int column, int w )
{
    Q3ListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) : w );
}

namespace
{
    bool HasMissingLogisticLinks( const Entity_ABC& entity )
    {
        const LogisticBaseStates* pHierarchy = static_cast< const LogisticBaseStates* >( entity.Retrieve< LogisticHierarchiesBase >() );
        if( !pHierarchy )
            return false;
        if( entity.GetTypeName() == kernel::Automat_ABC::typeName_ )
            return !pHierarchy->HasLogisticBaseSuperior();
        if( const LogisticLevelAttritube* attribute = entity.Retrieve< LogisticLevelAttritube >() )
            if( attribute->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
                return !pHierarchy->HasLogisticBaseSuperior();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Display
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void TacticalListView::Display( const Entity_ABC& entity, ValuedListItem* item )
{
    if( !item )
        return;
    item->setRenameEnabled( 0, true );
    UpdatePixmap( entity, item );
    QColor color = Qt::transparent;
    if( HasMissingLogisticLinks( entity ) && tools_.ShouldDisplay( "MissingLogisticLinks" ) )
        color = QColor( controllers_.options_.GetOption( "Color/MissingLogisticLinks", QString( "" ) ).To< QString >() );
    if( dynamic_cast< const Ghost_ABC* >( &entity ) != 0 )
        color = QColor( controllers_.options_.GetOption( "Color/Phantom", QString( "" ) ).To< QString >() );
    item->SetBackgroundColor( color );
    HierarchyListView< kernel::TacticalHierarchies >::Display( entity, item );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: SBO 2006-08-18
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const AutomatDecisions& decisions )
{
    const Entity_ABC* agent = & decisions.GetAgent();
    ValuedListItem* item = FindItem( agent, firstChild() );
    if( agent )
        UpdatePixmap( *agent, item );
}


namespace
{
    bool UpdateFormationRenamingText( const Entity_ABC& entity, ValuedListItem& item )
    {
        if( const Formation* formation = dynamic_cast< const Formation* >( &entity ) )
        {
            item.SetRenamingText( 0, formation->GetBasicName() );
            return true;
        }
        return false;
    }

    void UpdateHierarchieFormationRenamingText( const Entity_ABC& entity, Q3ListViewItem* firstItem )
    {
        if( ValuedListItem* item = FindItem( &entity, firstItem ) )
            if( !UpdateFormationRenamingText( entity, *item ) )
            {
                Q3ListViewItemIterator it( firstItem );
                while( it.current() )
                {
                    item = static_cast< ValuedListItem* >( it.current() );
                    if( item && item->GetValue< const Entity_ABC >() )
                        UpdateFormationRenamingText( *item->GetValue< const Entity_ABC >(), *item );
                    ++it;
                }
            }
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const Entity_ABC& entity )
{
    if( ValuedListItem* item = FindItem( &entity, firstChild() ) )
    {
        item->SetNamed( entity );
        UpdatePixmap( entity, item );
        UpdateFormationRenamingText( entity, *item );
        if( item->parent() )
            item->parent()->sort();
        else if( item->listView() )
            item->listView()->sort();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyCreated
// Created: MMC 2012-01-12
// -----------------------------------------------------------------------------
void TacticalListView::NotifyCreated( const kernel::TacticalHierarchies& hierarchy )
{
    HierarchyListView< kernel::TacticalHierarchies >::NotifyCreated( hierarchy );
    UpdateHierarchieFormationRenamingText( static_cast< const Hierarchies* >( &hierarchy )->GetEntity(), firstChild() );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: MMC 2012-01-12
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const kernel::TacticalHierarchies& hierarchy )
{
    HierarchyListView< kernel::TacticalHierarchies >::NotifyUpdated( hierarchy );
    UpdateHierarchieFormationRenamingText( static_cast< const Hierarchies* >( &hierarchy )->GetEntity(), firstChild() );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyCreated
// Created: MMC 2012-01-12
// -----------------------------------------------------------------------------
void TacticalListView::NotifyCreated( const kernel::Formation_ABC& entity )
{
    if( ValuedListItem* item = FindItem( &entity, firstChild() ) )
        UpdateFormationRenamingText( entity, *item );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::UpdatePixmap
// Created: LGY 2011-12-07
// -----------------------------------------------------------------------------
void TacticalListView::UpdatePixmap( const kernel::Entity_ABC& entity, gui::ValuedListItem* item )
{
    if( item )
    {
        item->setPixmap( 1, QPixmap() );
        if( const AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >() )
            if( decisions->IsEmbraye() )
                item->setPixmap( 1, lock_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::keyPressEvent
// Created: SBO 2006-09-05
// -----------------------------------------------------------------------------
void TacticalListView::keyPressEvent( QKeyEvent* event )
{
    if( selectedItem() && event->key() == Qt::Key_Delete )
        modelBuilder_.DeleteEntity( *( ( ValuedListItem* )selectedItem() )->GetValue< const Entity_ABC >() );
    else
        Q3ListView::keyPressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::OnContextMenuRequested
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void TacticalListView::OnContextMenuRequested( Q3ListViewItem* item, const QPoint& pos, int index )
{
    if ( isRenaming() )
        static_cast< RichListItem* >( item )->ValidUserRenaming( 0 );

    HierarchyListView_ABC::OnContextMenuRequested( item, pos, index );
    if( item || !isVisible() )
        return;
    modelBuilder_.ClearSelection();
    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    menu->insertItem( tr( "Create side" ), &modelBuilder_, SLOT( OnCreate() ) );
    menu->exec( pos, index );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Entity_ABC& entity, ContextMenu& menu )
{
    contextMenuEntity_ = &entity;
    if( !isVisible() || !IsActivated() )
        return;
    menu.InsertItem( "Command", tr( "Rename" ), this, SLOT( OnRename() ), false, 4 );
    if( entity.Get< kernel::TacticalHierarchies >().GetSuperior() != 0 )
        menu.InsertItem( "Command", tr( "Change superior" ), this, SLOT( OnChangeSuperior() ), false, 1 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Team_ABC& team, ContextMenu& menu )
{
    contextMenuEntity_ = &team;
    if( !isVisible() || !IsActivated() )
        return;
    AddFormationMenu( menu, eNatureLevel_xxxxx );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Formation_ABC& formation, ContextMenu& menu )
{
    contextMenuEntity_ = &formation;
    if( !isVisible() || !IsActivated() )
        return;
    if( formation.GetLevel() > eNatureLevel_c )
        AddFormationMenu( menu, static_cast< E_NatureLevel >( formation.GetLevel() ) );

    Q3PopupMenu* subMenu = menu.SubMenu( "Helpers", tr( "Change hierarchy level" ), false, 4 );
    for( int level = static_cast< int >( eNatureLevel_xxxxx ); level > 0; level-- )
        subMenu->insertItem( ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( level ), ENT_Tr_ABC::eToTr ).c_str(), this, SLOT( OnChangeLevel( int ) ), 0, level );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    contextMenuEntity_ = &agent;
    if( !isVisible() || !IsActivated() )
        return;
    if( const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >() )
    {
        if( ! decisions->IsEmbraye() )
            menu.InsertItem( "Helpers", tr( "Engage" ), this, SLOT( Engage() ), false, 0 );
        else if( decisions->CanBeOrdered() )
            menu.InsertItem( "Helpers", tr( "Disengage" ), this, SLOT( Disengage() ), false, 0 );
    }
    menu.InsertItem( "Command", tr( "Change automat type" ), this, SLOT( ChangeAutomatType() ), false, 0 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::AddFormationMenu
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void TacticalListView::AddFormationMenu( ContextMenu& menu, E_NatureLevel root )
{
    kernel::ContextMenu* subMenu = menu.SubMenu( "Command", tr( "Create formation" ), false, 3 );
    for( int level = static_cast< int >( root ); level > 0; level-- )
        subMenu->insertItem( ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( level ), ENT_Tr_ABC::eToTr ).c_str(), &modelBuilder_, SLOT( OnCreateFormation( int ) ), 0, level );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyContextMenu
// Created: ABR 2012-07-03
// -----------------------------------------------------------------------------
void TacticalListView::NotifyContextMenu( const kernel::Ghost_ABC& ghost, kernel::ContextMenu& menu )
{
    if( ghost.GetGhostType() != eGhostType_Automat || !isVisible() || !IsActivated() )
        return;
    contextMenuEntity_ = &ghost;
    menu.InsertItem( "Command", tr( "Replace by a new automat" ), this, SLOT( ChangeAutomatType() ), false, 6 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::OnChangeLevel
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
void TacticalListView::OnChangeLevel( int levelId )
{
    if( contextMenuEntity_ )
    {
        if( contextMenuEntity_->GetTypeName() != Formation_ABC::typeName_ )
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
// Name: TacticalListView::OnRename
// Created: SBO 2007-11-09
// -----------------------------------------------------------------------------
void TacticalListView::OnRename()
{
    if( selectedItem() )
        selectedItem()->startRename( 0 );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::OnChangeSuperior
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void TacticalListView::OnChangeSuperior()
{
    if( ValuedListItem* valuedItem = static_cast< ValuedListItem* >( selectedItem() ) ) // $$$$ ABR 2012-07-03: Use ContextMenuEntity instead, could operate on the wrong entity otherwise
    {
        Entity_ABC& entity = *valuedItem->GetValue< Entity_ABC >();
        if( !changeSuperiorDialog_ )
            changeSuperiorDialog_ = new gui::ChangeSuperiorDialog( this, controllers_, *this, false );
        changeSuperiorDialog_->Show( entity );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Engage
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void TacticalListView::Engage()
{
    if( ValuedListItem* valuedItem = static_cast< ValuedListItem* >( selectedItem() ) ) // $$$$ ABR 2012-07-03: Use ContextMenuEntity instead, could operate on the wrong entity otherwise
    {
        Entity_ABC& entity = *valuedItem->GetValue< Entity_ABC >();
        if( AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >() )
            decisions->Engage();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Disengage
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void TacticalListView::Disengage()
{
    if( ValuedListItem* valuedItem = static_cast< ValuedListItem* >( selectedItem() ) ) // $$$$ ABR 2012-07-03: Use ContextMenuEntity instead, could operate on the wrong entity otherwise
    {
        Entity_ABC& entity = *valuedItem->GetValue< Entity_ABC >();
        if( AutomatDecisions* decisions = entity.Retrieve< AutomatDecisions >() )
            decisions->Disengage();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::ChangeAutomatType
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
void TacticalListView::ChangeAutomatType()
{
    std::string typeName = "";
    if( contextMenuEntity_->GetTypeName() == kernel::Automat_ABC::typeName_ )
        typeName = static_cast< const kernel::Automat_ABC& >( *contextMenuEntity_ ).GetType().GetName();
    else if( contextMenuEntity_->GetTypeName() == kernel::Ghost_ABC::typeName_ )
        typeName = static_cast< const kernel::Ghost_ABC& >( *contextMenuEntity_ ).GetType();
    else
        return;
    ChangeAutomatTypeDialog( this, controllers_, agentTypes_, modelBuilder_, itemFactory_, *contextMenuEntity_.ConstCast(), typeName );
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
// Name: TacticalListView::CanChangeSuperior
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
bool TacticalListView::CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const
{
    if( dynamic_cast< const Agent_ABC* >( &entity ) )
        return dynamic_cast< const Automat_ABC* >( &superior ) != 0;
    if( dynamic_cast< const Automat_ABC* >( &entity ) )
        return dynamic_cast< const Formation_ABC* >( &superior ) != 0;
    if( dynamic_cast< const Formation_ABC* >( &entity ) )
        return ( !IsSubordinateOf( entity, superior ) && dynamic_cast< const Formation_ABC* >( &superior ) != 0 ) || dynamic_cast< const Team_ABC* >( &superior ) != 0 ;
    if( const Ghost_ABC* ghost = dynamic_cast< const Ghost_ABC* >( &entity ) )
        return ( ghost->GetGhostType() == eGhostType_Automat && dynamic_cast< const Formation_ABC* >( &superior ) != 0 )
            || ( ghost->GetGhostType() == eGhostType_Agent && dynamic_cast< const Team_ABC* >( &superior ) != 0 );
    if( dynamic_cast< const KnowledgeGroup_ABC* >( &entity ) )
        return dynamic_cast< const Formation_ABC* >( &superior ) != 0;
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::DoChangeSuperior
// Created: JSR 2011-11-08
// -----------------------------------------------------------------------------
void TacticalListView::DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior )
{
    Drop( entity, superior );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const Entity_ABC& item, const Entity_ABC& target )
{
    if( item.GetId() == target.GetId() )
        return false;
    if( const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( &item ) )
        return Drop( *agent, target );
    else if( const Automat_ABC* automat = dynamic_cast< const Automat_ABC* >( &item ) )
        return Drop( *automat, target );
    else if( const Formation_ABC* formation = dynamic_cast< const Formation_ABC* >( &item ) )
        return Drop( *formation, target );
    else if( const Ghost_ABC* ghost = dynamic_cast< const Ghost_ABC* >( &item ) )
        return Drop( *ghost, target );
    else if( const KnowledgeGroup_ABC* knowledgeGroup = dynamic_cast< const KnowledgeGroup_ABC* >( &item ) )
        return Drop( *knowledgeGroup, target );
    return false;
}

namespace
{
    Entity_ABC& GetFirstCommunicationChild( const Entity_ABC& entity )
    {
        tools::Iterator< const Entity_ABC& > it = entity.Get< CommunicationHierarchies >().CreateSubordinateIterator();
        if( it.HasMoreElements() )
            return const_cast< Entity_ABC& >( it.NextElement() );
        throw std::runtime_error( "No communication child found" );
    }

    void UpdateCommunicationHierarchies( Entity_ABC& entity, const Entity_ABC& superior )
    {
        const Entity_ABC& tacticalTop = superior.Get< kernel::TacticalHierarchies >().GetTop();
        CommunicationHierarchies* com = entity.Retrieve< CommunicationHierarchies >();

        if( com && &com->GetTop() != &tacticalTop )
            static_cast< ::EntityCommunications* >( com )->ChangeSuperior( GetFirstCommunicationChild( tacticalTop ) );

        tools::Iterator< const Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
        while( it.HasMoreElements() )
            UpdateCommunicationHierarchies( const_cast< Entity_ABC& >( it.NextElement() ), superior );
    }

    bool ChangeSuperior( const Entity_ABC& entity, const Entity_ABC& superior ) // $$$$ SBO 2006-09-28: cast-machine
    {
        Entity_ABC& ent = const_cast< Entity_ABC& >( entity );
        if( kernel::TacticalHierarchies* hierarchies = ent.Retrieve< kernel::TacticalHierarchies >() )
        {
            UpdateCommunicationHierarchies( ent, superior );
            static_cast< ::TacticalHierarchies* >( hierarchies )->ChangeSuperior( const_cast< Entity_ABC& >( superior ) );
            if( ProfileHierarchies_ABC* profileHierarchie = ent.Retrieve< ProfileHierarchies_ABC >() )
                static_cast< ::ProfileHierarchies* >( profileHierarchie )->ChangeSuperior( const_cast< Entity_ABC& >( superior ) );
            return true;
         }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const Agent_ABC& item, const Entity_ABC& target )
{
    const Automat_ABC* automat = dynamic_cast< const Automat_ABC* >( &target );
    if( automat )
    {
        bool superiorChange = ChangeSuperior( item, target );
        CommunicationHierarchies* com = const_cast< Agent_ABC& >( item ).Retrieve< CommunicationHierarchies >();
        if( com )
            static_cast< ::EntityCommunications* >( com )->ChangeSuperior( const_cast< Entity_ABC& >( target ) );
        return superiorChange;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const Automat_ABC& item, const Entity_ABC& target )
{
    const Formation_ABC* formation = dynamic_cast< const Formation_ABC* >( &target );
    if( formation )
        return ChangeSuperior( item, target );
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: SBO 2006-09-28
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const Formation_ABC& formation, const Entity_ABC& target )
{
    const Formation_ABC* targetFormation = dynamic_cast< const Formation_ABC* >( &target );
    if( targetFormation && !IsSubordinateOf( formation, *targetFormation ) )
        return ChangeSuperior( formation, target );

    const Team_ABC* team = dynamic_cast< const Team_ABC* >( &target );
    if( team )
        return ChangeSuperior( formation, target );
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: ABR 2011-10-20
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const kernel::Ghost_ABC& item, const kernel::Entity_ABC& target )
{
    const Formation_ABC* formation = dynamic_cast< const Formation_ABC* >( &target );
    if( formation && item.GetGhostType() == eGhostType_Automat )
        return ChangeSuperior( item, target );

    const Automat_ABC* automat = dynamic_cast< const Automat_ABC* >( &target );
    if( automat && item.GetGhostType() == eGhostType_Agent )
        return ChangeSuperior( item, target );

    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target )
{
    if( const Formation_ABC* formation = dynamic_cast< const Formation_ABC* >( &target ) )
    {
        tools::Iterator< const kernel::Entity_ABC& > children = item.Get< CommunicationHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
            ChangeSuperior( children.NextElement(), *formation );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: ABR 2012-07-18
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const kernel::AgentType& type, kernel::Entity_ABC& target )
{
    const geometry::Point2f position = target.Get< kernel::Positions >().GetPosition();
    if( Entity_ABC* result = model_.agents_.CreateAgent( static_cast< kernel::Ghost_ABC& >( target ), type, position ) )
    {
        delete static_cast< const Ghost_ABC* >( &target );
        setFocus();
        result->Select( controllers_.actions_ );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Drop
// Created: ABR 2012-07-18
// -----------------------------------------------------------------------------
bool TacticalListView::Drop( const kernel::AutomatType& type, kernel::Entity_ABC& target )
{
    const geometry::Point2f position = target.Get< kernel::Positions >().GetPosition();
    if( Entity_ABC* result = model_.agents_.CreateAutomatInsteadOf( target, type, position ) )
    {
        delete static_cast< const Ghost_ABC* >( &target );
        setFocus();
        result->Select( controllers_.actions_ );
        return true;
    }
    return false;
}
