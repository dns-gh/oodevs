// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "KnowledgeAddInGroupDialog.h"
#include "moc_KnowledgeAddInGroupDialog.cpp"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Identifier.h"
#include "actions/KnowledgeGroupMagicAction.h"
#include "actions/Enumeration.h"
#include "gaming/StaticModel.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog constructor
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
KnowledgeAddInGroupDialog::KnowledgeAddInGroupDialog( QWidget* pParent, kernel::Controllers& controllers, const kernel::Time_ABC& simulation, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel )
    : QDialog( pParent, 0, 0, Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Title | Qt::WStyle_SysMenu )
    , controllers_            ( controllers )
    , simulation_             ( simulation )
    , actionsModel_           ( actionsModel )
    , static_                 ( staticModel )
    , selectedKnowledgeGroup_ ( controllers )
    , pTempTarget_            ( 0 )
    , pSelectedTarget_        ( 0 )
    , pTargetName_            ( 0 )
{
    setCaption( tools::translate( "KnowledgeAddInGroupDialog", "Add to knowledge group" ) );

    QGridLayout* grid = new QGridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 10 );
    grid->setColStretch( 0, 3 );

    {
        pTargetNameTitle_ = new QLabel( this );
        pTargetNameTitle_->setText(  tr( "Target: " ) );
        grid->addWidget( pTargetNameTitle_, 0, 0 );

        pTargetName_ = new QLabel( this );
        pTargetName_->setText( "---" );
        grid->addWidget( pTargetName_, 0, 1 );
    }
    {
        grid->addWidget( new QLabel( tr( "Perception: " ), this ), 1, 0 );
        pPerceptionCombo_ = new QComboBox( this );
        pPerceptionCombo_->addItem( tools::ToString( kernel::eDetection ), sword::UnitIdentification::detected );
        pPerceptionCombo_->addItem( tools::ToString( kernel::eRecognition ), sword::UnitIdentification::recognized );
        pPerceptionCombo_->addItem( tools::ToString( kernel::eIdentification ), sword::UnitIdentification::identified );
        grid->addWidget( pPerceptionCombo_, 1, 1 );
    }
    {
        QPushButton* ok     = new QPushButton( tools::translate( "KnowledgeAddInGroupDialog", "Ok" ), this );
        QPushButton* cancel = new QPushButton( tools::translate( "KnowledgeAddInGroupDialog", "Cancel" ), this );
        grid->addWidget( ok, 2, 1 );
        grid->addWidget( cancel, 2, 2 );
        connect( ok, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( OnReject() ) );
    }

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog destructor
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
KnowledgeAddInGroupDialog::~KnowledgeAddInGroupDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::Show
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::Show( kernel::SafePointer< kernel::KnowledgeGroup_ABC > knowledgeGroup )
{
    show();
    selectedKnowledgeGroup_ = knowledgeGroup;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::OnAccept
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::OnAccept()
{
    if( !pSelectedTarget_ )
    {
        pTargetNameTitle_->setText( "<font color=\"#FF0000\">" + tr( "Target: " ) + "</font>" );
        return;
    }

    int selectedPerception = pPerceptionCombo_->itemData( pPerceptionCombo_->currentItem() ).toInt();

    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "knowledge_group_add_knowledge" );
    actions::KnowledgeGroupMagicAction* action = new actions::KnowledgeGroupMagicAction( actionType, controllers_.controller_, true );
    action->Rename( tools::translate( "gaming_app::Action", "Knowledge Group Add knowledge" ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, selectedKnowledgeGroup_, false ) );
    action->AddParameter( *new actions::parameters::Identifier( it.NextElement(), pSelectedTarget_->GetId() ) );
    action->AddParameter( *new actions::parameters::Enumeration( it.NextElement(), selectedPerception ) );
    actionsModel_.Publish( *action );

    Close();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::OnReject
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::OnReject()
{
    Close();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::Close
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::Close()
{
    pTempTarget_ = 0;
    pSelectedTarget_ = 0;

    if( pPerceptionCombo_ )
        pPerceptionCombo_->setCurrentItem( 0 );
    if( pTargetNameTitle_ )
        pTargetNameTitle_->setText(  tr( "Target: " ) );
    if( pTargetName_ )
        pTargetName_->setText( "---" );

    hide();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::NotifyContextMenu
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    InsertInMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::NotifyContextMenu
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    InsertInMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::NotifyContextMenu
// Created: JSR 2012-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::NotifyContextMenu( const kernel::UrbanObject_ABC& entity, kernel::ContextMenu& menu )
{
    InsertInMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::NotifyContextMenu
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    InsertInMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::InsertInMenu
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::InsertInMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( !isHidden() )
    {
        pTempTarget_ = &entity;
        menu.InsertItem( "Command", tr( "Add to knowledge group" ), this, SLOT( SetTarget() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::SetTarget
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::SetTarget()
{
    if( pTempTarget_ )
    {
        pSelectedTarget_ = pTempTarget_;
        pTargetName_->setText( pSelectedTarget_->GetName() );
        if( pTargetNameTitle_ )
            pTargetNameTitle_->setText( tr( "Target: " ) );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeAddInGroupDialog::closeEvent
// Created: MMC 2011-06-06
// -----------------------------------------------------------------------------
void KnowledgeAddInGroupDialog::closeEvent( QCloseEvent* /*e*/ )
{
    Close();
}
