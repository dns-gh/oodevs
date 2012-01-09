// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FormationHierarchyEditor.h"
#include "moc_FormationHierarchyEditor.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/tools.h"
#include "preparation/Formation.h"

// -----------------------------------------------------------------------------
// Name: FormationHierarchyEditor constructor
// Created: MMC 2012-01-05
// -----------------------------------------------------------------------------
FormationHierarchyEditor::FormationHierarchyEditor( QWidget* parent, kernel::Controllers& controllers, const kernel::FormationLevels& levels )
    : QObject( parent )
    , controllers_( controllers )
    , selected_   ( controllers )
    , levels_     ( levels )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchyEditor destructor
// Created: MMC 2012-01-05
// -----------------------------------------------------------------------------
FormationHierarchyEditor::~FormationHierarchyEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchyEditor::NotifyContextMenu
// Created: MMC 2012-01-05
// -----------------------------------------------------------------------------
void FormationHierarchyEditor::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    const Formation* pFormation = dynamic_cast< const Formation* >( &entity );
    if ( !pFormation )
        return;
    
    selected_ = pFormation;
    Q3PopupMenu* subMenu = menu.SubMenu( "Creation", tr( "Change hierarchy level" ) );
    for( const kernel::HierarchyLevel_ABC* level = levels_.Resolve( "xxxxx" ); level; level = level->GetNext() )
        subMenu->insertItem( tools::findTranslation( "models::app6", level->GetName().ascii() ),this, SLOT( OnChangeLevel( int ) ), 0, level->GetId() );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchyEditor::OnChangeLevel
// Created: MMC 2012-01-05
// -----------------------------------------------------------------------------
void FormationHierarchyEditor::OnChangeLevel( int levelId )
{
    if( selected_ )
    {
        selected_.ConstCast()->SetLevel( levelId );
        UpdateHierarchies();
    }
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchyEditor::UpdateHierarchies
// Created: MMC 2012-01-05
// -----------------------------------------------------------------------------
void FormationHierarchyEditor::UpdateHierarchies()
{
    if( kernel::TacticalHierarchies* pTactical = selected_.ConstCast()->Retrieve< kernel::TacticalHierarchies >() )
    {
        pTactical->UpdateSymbol();
        controllers_.controller_.Update( *pTactical );
    }
}
