// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticLinksEditor.h"
#include "moc_LogisticLinksEditor.cpp"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/tools.h"
#include "preparation/LinkGenerator.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor constructor
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
LogisticLinksEditor::LogisticLinksEditor( QWidget* parent, kernel::Controllers& controllers )
    : QObject( parent )
    , controllers_           ( controllers )
    , selected_              ( controllers )
    , pGenerator_            ( new LinkGenerator() )
    , pAddTeamMapper_        ( new QSignalMapper( this ) )
    , pAddFormationMapper_   ( new QSignalMapper( this ) )
    , pAddAutomatMapper_     ( new QSignalMapper( this ) )
    , pRemoveTeamMapper_     ( new QSignalMapper( this ) )
    , pRemoveFormationMapper_( new QSignalMapper( this ) )
    , pRemoveAutomatMapper_  ( new QSignalMapper( this ) )
{
    connect( pAddTeamMapper_.get(), SIGNAL( mapped( int ) ), this, SLOT( Generate() ) );
    connect( pAddFormationMapper_.get(), SIGNAL( mapped( int ) ), this, SLOT( GenerateFromFormation() ) );
    connect( pAddAutomatMapper_.get(), SIGNAL( mapped( int ) ), this, SLOT( GenerateFromAutomat() ) );
    connect( pRemoveTeamMapper_.get(), SIGNAL( mapped( int ) ), this, SLOT( Remove() ) );
    connect( pRemoveFormationMapper_.get(), SIGNAL( mapped( int ) ), this, SLOT( RemoveFromFormation() ) );
    connect( pRemoveAutomatMapper_.get(), SIGNAL( mapped( int ) ), this, SLOT( RemoveFromAutomat() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor destructor
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
LogisticLinksEditor::~LogisticLinksEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::NotifyContextMenu
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LogisticLinksEditor::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    if( automat.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
    {
        Create( tools::translate( "LogisticLinksEditor", "Assign logistic subordinates" ), automat, menu, *pAddAutomatMapper_ );
        Create( tools::translate( "LogisticLinksEditor", "Remove logistic subordinates" ), automat, menu, *pRemoveAutomatMapper_ );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::NotifyContextMenu
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LogisticLinksEditor::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    if( formation.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
    {
        Create( tools::translate( "LogisticLinksEditor", "Assign logistic subordinates" ), formation, menu, *pAddFormationMapper_ );
        Create( tools::translate( "LogisticLinksEditor", "Remove logistic subordinates" ), formation, menu, *pRemoveFormationMapper_ );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::NotifyContextMenu
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------q
void LogisticLinksEditor::NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu )
{
    Create( tools::translate( "LogisticLinksEditor", "Generate logistic links" ), team, menu, *pAddTeamMapper_ );
    Create( tools::translate( "LogisticLinksEditor", "Remove logistic links" ), team, menu, *pRemoveTeamMapper_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::Create
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LogisticLinksEditor::Create( const QString& title, const kernel::Entity_ABC& entity,
                            kernel::ContextMenu& menu, QSignalMapper& mapper )
{
    selected_ = &entity;
    kernel::ContextMenu* pSubMenu = menu.SubMenu( "Logistic", tr( "Logistic" ) );
    mapper.setMapping( pSubMenu->addAction( title, &mapper, SLOT( map() ) ), 0 );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::Generate
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LogisticLinksEditor::Generate()
{
    if( selected_ )
        pGenerator_->Generate( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::GenerateFromAutomat
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LogisticLinksEditor::GenerateFromAutomat()
{
    if( selected_ )
        pGenerator_->GenerateFromAutomat( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::GenerateFromFormation
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LogisticLinksEditor::GenerateFromFormation()
{
    if( selected_ )
        pGenerator_->GenerateFromFormation( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::Remove
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
void LogisticLinksEditor::Remove()
{
    if( selected_ )
        pGenerator_->Remove( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::RemoveFromAutomat
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
void LogisticLinksEditor::RemoveFromAutomat()
{
    if( selected_ )
        pGenerator_->RemoveFromAutomat( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::RemoveFromFormation
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
void LogisticLinksEditor::RemoveFromFormation()
{
    if( selected_ )
        pGenerator_->RemoveFromFormation( *selected_ );
}
