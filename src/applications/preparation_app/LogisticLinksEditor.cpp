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
    , controllers_     ( controllers )
    , selected_        ( controllers )
    , pGenerator_      ( new LinkGenerator() )
    , pTeamMapper_     ( new QSignalMapper( this ) )
    , pFormationMapper_( new QSignalMapper( this ) )
    , pAutomatMapper_  ( new QSignalMapper( this ) )
{
    connect( pTeamMapper_.get(), SIGNAL( mapped( int ) ), this, SLOT( Generate() ) );
    connect( pFormationMapper_.get(), SIGNAL( mapped( int ) ), this, SLOT( GenerateFromFormation() ) );
    connect( pAutomatMapper_.get(), SIGNAL( mapped( int ) ), this, SLOT( GenerateFromAutomat() ) );
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
        Create( tools::translate( "LogisticLinksEditor", "Assign logistic subordinates" ), automat, menu, *pAutomatMapper_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::NotifyContextMenu
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LogisticLinksEditor::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    if( formation.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        Create( tools::translate( "LogisticLinksEditor", "Assign logistic subordinates" ), formation, menu, *pFormationMapper_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::NotifyContextMenu
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LogisticLinksEditor::NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu )
{
    Create( tools::translate( "LogisticLinksEditor", "Generate logistic links" ), team, menu, *pTeamMapper_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinksEditor::Create
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LogisticLinksEditor::Create( const QString& title, const kernel::Entity_ABC& entity,
                            kernel::ContextMenu& menu, QSignalMapper& mapper )
{
    selected_ = const_cast< kernel::Entity_ABC* >( &entity );
    Q3PopupMenu* pSubMenu = menu.SubMenu( "Logistic", tr( "Logistic" ) );
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
