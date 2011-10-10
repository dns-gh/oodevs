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
#include "clients_kernel/Team_ABC.h"
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
    , config_     ( config )
    , pFactory_   ( new kernel::SymbolFactory() )
    , selected_   ( controllers )
    , menu_       ( 0 )
{
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

// -----------------------------------------------------------------------------
// Name: SymbolEditor::NotifyUpdated
// Created: JSR 2011-08-02
// -----------------------------------------------------------------------------
void SymbolEditor::NotifyUpdated( const kernel::ModelLoaded& )
{
    pFactory_->Load( config_ );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::NotifyUpdated
// Created: JSR 2011-08-02
// -----------------------------------------------------------------------------
void SymbolEditor::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    pFactory_->Unload();
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

namespace
{
    class MenuStyle : public QProxyStyle
    {
    public:
        MenuStyle()
        {
            // NOTHING
        }
        ~MenuStyle()
        {
            // NOTHING
        }
        virtual int pixelMetric( PixelMetric metric, const QStyleOption* option = 0, const QWidget * widget = 0 ) const
        {
            if( metric == PM_SmallIconSize)
                return 50;
            return QProxyStyle::pixelMetric( metric, option, widget );
        }
    };
}
// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: LGY 2011-07-21
// -----------------------------------------------------------------------------
void SymbolEditor::Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    menu_ = menu.SubMenu( "Symbols", tr( "Symbol" ) );
    menu_->setStyle( new MenuStyle() );
    selected_ = const_cast< kernel::Entity_ABC* >( &entity );
    Update();
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::OnChangeSymbol
// Created: LGY 2011-07-27
// -----------------------------------------------------------------------------
void SymbolEditor::OnChangeSymbol( QAction* action )
{
    if( selected_ )
    {
        if( action->data().isNull() )
            selected_.ConstCast()->Get< kernel::SymbolHierarchy_ABC >().Reset();
        else
            selected_.ConstCast()->Get< kernel::SymbolHierarchy_ABC >().OverrideValue( action->data().toString().toStdString() );
        UpdateHierarchies();
    }
    menu_ = 0;
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: LGY 2011-07-27
// -----------------------------------------------------------------------------
void SymbolEditor::Update()
{
    if( !selected_ || !menu_ )
        return;
    const kernel::TacticalHierarchies& pHierarchy = selected_->Get< kernel::TacticalHierarchies >();
    const std::string karma = Convert( pHierarchy.GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
    std::set< std::string > symbols;
    tools::Iterator< const kernel::Entity_ABC& > it = pHierarchy.CreateSubordinateIterator();
    while( it.HasMoreElements() )
        pFactory_->FillSymbols( it.NextElement().Get< kernel::TacticalHierarchies >().GetSymbol(), karma, symbols );
    T_Symbols pixmaps;
    BOOST_FOREACH( const std::string& symbol, symbols )
        pixmaps[ symbol ] = symbols_.GetSymbol( *selected_, symbol, pHierarchy.GetLevel() );
    if( ! IsValid( pixmaps ) )
        QTimer::singleShot( 100, this, SLOT( Update() ) );
    else
    {
        bool overriden = selected_->Get< kernel::SymbolHierarchy_ABC >().IsOverriden();
        BOOST_FOREACH( const T_Symbols::value_type& pixmap, pixmaps )
        {
            std::string displayString = pixmap.first;
            std::size_t position = displayString.rfind( '/' );
            if( position != std::string::npos )
                displayString = displayString.substr( position + 1 );
            QAction* action = menu_->addAction( QIcon( pixmap.second ), displayString.c_str() );
            action->setData( QVariant( pixmap.first.c_str() ) );
            if( overriden && pHierarchy.GetSymbol() == pixmap.first )
                action->setEnabled( false );
        }
        if( overriden )
            menu_->addAction( tr( "Default" ) );
        connect( menu_, SIGNAL( triggered( QAction* ) ), this, SLOT( OnChangeSymbol( QAction* ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::NotifyUpdated
// Created: LGY 2011-10-10
// -----------------------------------------------------------------------------
void SymbolEditor::NotifyUpdated( const kernel::Team_ABC& team )
{
    selected_ = &team;
    UpdateHierarchies();
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::UpdateHierarchies
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
void SymbolEditor::UpdateHierarchies()
{
    if( kernel::TacticalHierarchies* pTactical = selected_.ConstCast()->Retrieve< kernel::TacticalHierarchies >() )
    {
        pTactical->UpdateSymbol();
        controllers_.controller_.Update( *pTactical );
        if( const kernel::CommunicationHierarchies* pCommunication = selected_->Retrieve< kernel::CommunicationHierarchies >() )
            controllers_.controller_.Update( *pCommunication );
        else if( const kernel::CommunicationHierarchies* pCommunication = pTactical->GetTop().Retrieve< kernel::CommunicationHierarchies >() )
            controllers_.controller_.Update( *pCommunication );
    }
}
