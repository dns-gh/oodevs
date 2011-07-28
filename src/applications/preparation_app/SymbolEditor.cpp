// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SymbolEditor.h"
#include "moc_SymbolEditor.cpp"
#include "clients_gui/EntitySymbols.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/tools.h"
#include "tools/Iterator.h"
#include <boost/foreach.hpp>
#include <set>

// -----------------------------------------------------------------------------
// Name: SymbolEditor constructor
// Created: LGY 2011-07-21
// -----------------------------------------------------------------------------
SymbolEditor::SymbolEditor( QWidget* parent, kernel::Controllers& controllers, const gui::EntitySymbols& symbols,
                            const tools::ExerciseConfig& config )
    : QObject( parent )
    , controllers_( controllers )
    , symbols_    ( symbols )
    , pFactory_   ( new kernel::SymbolFactory() )
    , selected_   ( controllers )
    , menu_       ( 0 )
{
    pFactory_->Load( config );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor destructor
// Created: LGY 2011-07-21
// -----------------------------------------------------------------------------
SymbolEditor::~SymbolEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::NotifyContextMenu
// Created: LGY 2011-07-21
// -----------------------------------------------------------------------------
void SymbolEditor::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::NotifyContextMenu
// Created: LGY 2011-07-21
// -----------------------------------------------------------------------------
void SymbolEditor::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

namespace
{
    std::string Convert( const kernel::Karma& karma )
    {
        switch( karma.GetUId() )
        {
        case 1:
            return "f";
        case 2:
            return "h";
        case 3:
            return "n";
        default:
            return "u";
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::IsValid
// Created: LGY 2011-07-26
// -----------------------------------------------------------------------------
bool SymbolEditor::IsValid( const T_Symbols& symbols ) const
{
    BOOST_FOREACH( const T_Symbols::value_type& symbol, symbols )
        if( symbol.second.isNull() )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: LGY 2011-07-26
// -----------------------------------------------------------------------------
void SymbolEditor::Update()
{
    if( menu_ )
        Update( *selected_, menu_ );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: LGY 2011-07-21
// -----------------------------------------------------------------------------
void SymbolEditor::Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    menu_ = menu.SubMenu( "Symbols", tr( "Symbol" ) );
    selected_ = const_cast< kernel::Entity_ABC* >( &entity );
    Update();
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::OnChangeSymbol
// Created: LGY 2011-07-27
// -----------------------------------------------------------------------------
void SymbolEditor::OnChangeSymbol( int id )
{
    CIT_Identifier it = identifiers_.find( id );
    if( it != identifiers_.end() && selected_ )
    {
        const_cast< kernel::SymbolHierarchy_ABC* >( selected_->Retrieve< kernel::SymbolHierarchy_ABC >() )->SetValue( it->second );
        UpdateHierarchies( *selected_ );
    }
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: LGY 2011-07-27
// -----------------------------------------------------------------------------
void SymbolEditor::Update( const kernel::Entity_ABC& entity, QPopupMenu* menu )
{
    identifiers_.clear();
    const kernel::TacticalHierarchies& pHierarchy = entity.Get< kernel::TacticalHierarchies >();
    const std::string karma = Convert( pHierarchy.GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
    std::set< std::string > symbols;
    tools::Iterator< const kernel::Entity_ABC& > it = pHierarchy.CreateSubordinateIterator();
    while( it.HasMoreElements() )
        pFactory_->FillSymbols( it.NextElement().Get< kernel::TacticalHierarchies >().GetSymbol(), karma, symbols );
    T_Symbols pixmaps;
    BOOST_FOREACH( const std::string& symbol, symbols )
        pixmaps[ symbol ] = symbols_.GetSymbol( entity, symbol, pHierarchy.GetLevel(), QSize( 32, 32 ) );
    if( ! IsValid( pixmaps ) )
        QTimer::singleShot( 100, this, SLOT( Update() ) );
    else
        BOOST_FOREACH( const T_Symbols::value_type& pixmap, pixmaps )
        {
            int id = menu->insertItem( pixmap.second, this, SLOT( OnChangeSymbol( int ) ) );
            identifiers_[ id ] = pixmap.first;
            if( pHierarchy.GetSymbol() == pixmap.first )
                menu->setItemChecked( id, true );
        }
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::UpdateHierarchies
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
void SymbolEditor::UpdateHierarchies( const kernel::Entity_ABC& entity )
{
    if( const kernel::TacticalHierarchies* pTactical = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        controllers_.controller_.Update( *pTactical );
        if( const kernel::CommunicationHierarchies* pCommunication = entity.Retrieve< kernel::CommunicationHierarchies >() )
            controllers_.controller_.Update( *pCommunication );
        else if( const kernel::CommunicationHierarchies* pCommunication = pTactical->GetTop().Retrieve< kernel::CommunicationHierarchies >() )
            controllers_.controller_.Update( *pCommunication );
    }
}
