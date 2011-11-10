// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "GhostSymbolEditor.h"
#include "moc_GhostSymbolEditor.cpp"

#include "clients_gui/NatureEditionCategory.h"
#include "clients_gui/NatureEditionWidget.h"
#include "clients_gui/UnitPreviewIcon.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolRule.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor constructor
// Created: ABR 2011-11-02
// -----------------------------------------------------------------------------
GhostSymbolEditor::GhostSymbolEditor( QGridLayout* layout, int row, kernel::Controllers& controllers, kernel::SymbolFactory& symbolsFactory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy )
    : QWidget( ( layout && layout->parentWidget() ) ? layout->parentWidget() : 0 )
    , symbolsFactory_( symbolsFactory )
{
    // Level
    layout->addWidget( new QLabel( tr( "Level:" ), this ), row, 0 );
    levelComboBox_ = new QComboBox( this );
    levelBase_ = tr( "Select a parent automat or formation" );
    connect( levelComboBox_, SIGNAL( activated( int ) ), SLOT( UpdateSymbol() ) );
    layout->addWidget( levelComboBox_, row, 1 );

    // Nature
    natureWidget_ = new gui::NatureEditionWidget( layout, row + 1, 2 );
    connect( natureWidget_, SIGNAL( textChanged( const QString& ) ), SLOT( UpdateSymbol() ) );

    // Icon
    icon_ = new gui::UnitPreviewIcon( this, controllers, icons, colorStrategy, tr( "Drag and drop symbol to map to create a new phantom." ) );
    layout->addWidget( icon_, row + 3, 0, 1, 2 );
    connect( icon_, SIGNAL( StartDrag() ), SIGNAL( StartDrag() ) );
    connect( icon_, SIGNAL( SelectionChanged( const kernel::Entity_ABC* ) ), SLOT( OnSelectionChanged( const kernel::Entity_ABC* ) ) );

    // Configuration
    for( unsigned int i = 0; i < unsigned int( eNbrNatureLevel ); ++i )
        levelNames_ << tools::ToString( static_cast< E_NatureLevel >( i ), ENT_Tr_ABC::eToSim );
    OnSelectionChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor destructor
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
GhostSymbolEditor::~GhostSymbolEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor::GetSymbol
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
const std::string& GhostSymbolEditor::GetSymbol() const
{
    return icon_->GetSymbol();
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor::GetNature
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
const std::string& GhostSymbolEditor::GetNature() const
{
    return nature_;
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor::GetLevel
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
const std::string& GhostSymbolEditor::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor::GetGhostType
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
E_GhostType GhostSymbolEditor::GetGhostType() const
{
    return ghostType_;
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor::IsLevelValid
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
bool GhostSymbolEditor::IsLevelValid() const
{
    return levelComboBox_->currentText() != levelBase_;
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor::UpdateSymbol
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void GhostSymbolEditor::UpdateSymbol()
{
    nature_ = natureWidget_->text().ascii();
    icon_->SetSymbol( symbolsFactory_.CreateSymbol( nature_ ) );
    if( levelComboBox_->currentText() != levelBase_ )
    {
        level_ = levelComboBox_->currentText().ascii();
        icon_->SetLevel( std::string( "levels/" ) + level_ );
    }
    else
    {
        level_ = "";
        icon_->SetLevel( "" );
    }
    icon_->UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor::FillLevelFromParent
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
void GhostSymbolEditor::FillLevelFromParent( const kernel::Entity_ABC* parent )
{
    E_NatureLevel parentLevel = eNatureLevel_None;

    if( parent )
    {
        const kernel::TacticalHierarchies* pHierarchy = parent->Retrieve< kernel::TacticalHierarchies >();
        if( pHierarchy )
        {
            std::string level = pHierarchy->GetLevel();
            level.erase( 0, 7 );
            parentLevel = tools::NatureLevelFromString( level );
        }
    }
    levelComboBox_->clear();
    for( int i = parentLevel - 1; i > eNatureLevel_None; --i )
        levelComboBox_->addItem( levelNames_[ i ] );
    if( levelComboBox_->count() == 0 )
        levelComboBox_->addItem( levelBase_ );
    emit( LevelChanged() );
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor::OnSelectionChanged
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void GhostSymbolEditor::OnSelectionChanged( const kernel::Entity_ABC* parent )
{
    ghostType_ = eGhostType_Invalid;

    if( parent )
        ghostType_ = ( parent->GetTypeName() == kernel::Automat_ABC::typeName_ ) ? eGhostType_Agent :
                    ( parent->GetTypeName() == kernel::Formation_ABC::typeName_ ) ? eGhostType_Automat :
                    eGhostType_Invalid;

    FillLevelFromParent( parent );
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor::Reset
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void GhostSymbolEditor::Reset()
{
    natureWidget_->Clear();
    natureWidget_->SetRootSymbolRule( *symbolsFactory_.GetSymbolRule() );
    natureWidget_->setText( "undefined/undefined/undefined/undefined" );
    OnSelectionChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: GhostSymbolEditor::Fill
// Created: ABR 2011-11-02
// -----------------------------------------------------------------------------
void GhostSymbolEditor::Fill( const kernel::Ghost_ABC& ghost )
{
    natureWidget_->Clear();
    natureWidget_->SetRootSymbolRule( *symbolsFactory_.GetSymbolRule() );
    natureWidget_->setText( "undefined/undefined/undefined/undefined" );
    {
        const kernel::TacticalHierarchies* pHierarchy = ghost.Retrieve< kernel::TacticalHierarchies >();
        assert( pHierarchy && pHierarchy->GetSuperior() );
        FillLevelFromParent( pHierarchy->GetSuperior() );
        assert( IsLevelValid() );
    }
    icon_->SetSelectedParent( &ghost ); // $$$$ ABR 2011-11-07: We want color and karma from the ghost, not from his parent
    UpdateSymbol();
    natureWidget_->setText( ghost.GetNature().c_str() );
}
