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
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/tools.h"
#include "preparation/Formation.h"
#include "ENT/ENT_Tr_Gen.h"

// -----------------------------------------------------------------------------
// Name: FormationHierarchyEditor constructor
// Created: MMC 2012-01-05
// -----------------------------------------------------------------------------
FormationHierarchyEditor::FormationHierarchyEditor( QWidget* parent, kernel::Controllers& controllers )
    : QObject( parent )
    , controllers_( controllers )
    , selected_   ( controllers )
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
    Q3PopupMenu* subMenu = menu.SubMenu( "Helpers", tr( "Change hierarchy level" ), false, 4 );

    for( int level = static_cast< int >( eNatureLevel_xxxxx ); level > 0; level-- )
        subMenu->insertItem( ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( level ), ENT_Tr_ABC::eToTr ).c_str(), this, SLOT( OnChangeLevel( int ) ), 0, level );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchyEditor::OnChangeLevel
// Created: MMC 2012-01-05
// -----------------------------------------------------------------------------
void FormationHierarchyEditor::OnChangeLevel( int levelId )
{
    if( selected_ )
    {
        selected_.ConstCast()->SetLevel( static_cast< E_NatureLevel >( levelId ) );
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
        pTactical->UpdateSymbolUpward();
        controllers_.controller_.Update( *pTactical );
    }
}
