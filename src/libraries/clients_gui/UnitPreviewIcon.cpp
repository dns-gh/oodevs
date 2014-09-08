// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitPreviewIcon.h"
#include "moc_UnitPreviewIcon.cpp"
#include "SymbolIcons.h"
#include "ColorStrategy_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_gui/MergingTacticalHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon constructor
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
UnitPreviewIcon::UnitPreviewIcon( QWidget* parent, Controllers& controllers, SymbolIcons& icons, ColorStrategy_ABC& colorStrategy, QString tooltips /* = QString()*/ )
    : Q3HBox          ( parent )
    , controllers_   ( controllers )
    , icons_         ( icons )
    , colorStrategy_ ( colorStrategy )
    , selectedParent_( controllers_ )
{
    setStyleSheet( "background-color: white" );
    setFrameStyle( QFrame::Plain | QFrame::Box );
    setMinimumSize( 140, 140 );
    layout()->setAlignment( Qt::AlignCenter | Qt::AlignHCenter );
    icon_ = new QLabel( this );
    icon_->setMargin( 5 );
    if( !tooltips.isEmpty() )
        QToolTip::add( icon_, tooltips );
    UpdateSymbol();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon destructor
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
UnitPreviewIcon::~UnitPreviewIcon()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::NotifySelected
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::NotifySelected( const AgentType& type )
{
    symbol_ = type.GetSymbol();
    level_ = type.GetLevelSymbol();
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::NotifySelected
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::NotifySelected( const AutomatType& type )
{
    symbol_ = type.GetSymbol();
    std::string levelSymbol = type.GetTypePC()->GetLevelSymbol();
    if( !levelSymbol.empty() )
        level_ = MergingTacticalHierarchies::IncreaseLevel( levelSymbol ); // $$$$ SBO 2007-10-16:
    else
        level_.erase();
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::BeforeSelection
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::BeforeSelection()
{
    selectedParent_ = 0;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::AfterSelection
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::AfterSelection()
{
    emit SelectionChanged( selectedParent_ );
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::Select
// Created: NLD 2010-10-13
// -----------------------------------------------------------------------------
void UnitPreviewIcon::Select( const kernel::Team_ABC& element )
{
    selectedParent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::Select
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::Select( const kernel::Formation_ABC& element )
{
    selectedParent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::Select
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::Select( const kernel::Automat_ABC& element )
{
    selectedParent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::Select
// Created: NLD 2010-10-13
// -----------------------------------------------------------------------------
void UnitPreviewIcon::Select( const kernel::Agent_ABC& element )
{
   selectedParent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::UpdateSymbol
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::UpdateSymbol()
{
    if( symbol_.empty() )
    {
        QPixmap blank( 128, 128 );
        blank.fill( Qt::white );
        icon_->setPixmap( blank );
    }
    else
    {
        if( selectedParent_ )
        {
            const TacticalHierarchies* pHierarchy = selectedParent_->Retrieve< TacticalHierarchies >();
            if( pHierarchy )
                kernel::App6Symbol::SetKarma( symbol_, pHierarchy->GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
        }
        SymbolIcon icon( symbol_, level_ );
        if( selectedParent_ )
            icon.SetColor( colorStrategy_.FindColor( *selectedParent_ ) );
        icon.SetSize( 128 );
        QPixmap img = icons_.GetSymbol( icon );
        if( !img.isNull() )
            icon_->setPixmap( img );
        else
            QTimer::singleShot( 100, this, SLOT( UpdateSymbol() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::mouseMoveEvent
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::mouseMoveEvent( QMouseEvent* event )
{
    if( selectedParent_ && ( event->button() | Qt::LeftButton ) )
        emit StartDrag();
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::SetSymbol
// Created: ABR 2011-10-17
// -----------------------------------------------------------------------------
void UnitPreviewIcon::SetSymbol( const std::string& symbol )
{
    symbol_ = symbol;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::SetLevel
// Created: ABR 2011-10-17
// -----------------------------------------------------------------------------
void UnitPreviewIcon::SetLevel( const std::string& level )
{
    level_ = level;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::GetSymbol
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
const std::string& UnitPreviewIcon::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::GetLevel
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
const std::string& UnitPreviewIcon::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::GetSelectedParent
// Created: ABR 2011-10-17
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* UnitPreviewIcon::GetSelectedParent() const
{
    return selectedParent_;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::SetSelectedParent
// Created: ABR 2011-11-07
// -----------------------------------------------------------------------------
void UnitPreviewIcon::SetSelectedParent( const kernel::Entity_ABC* entity )
{
    selectedParent_ = entity;
}
