// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "EntitySymbolEditor.h"
#include "moc_EntitySymbolEditor.cpp"

#include "clients_gui/NatureEditionCategory.h"
#include "clients_gui/NatureEditionWidget.h"
#include "clients_gui/UnitPreviewIcon.h"
#include "clients_gui/RichWidget.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolRule.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor constructor
// Created: ABR 2011-11-02
// -----------------------------------------------------------------------------
EntitySymbolEditor::EntitySymbolEditor( QGridLayout* layout, int row, kernel::Controllers& controllers, kernel::SymbolFactory& symbolsFactory,
                                        gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy, const QString& iconPreviewToolTips,
                                        int stretch )
    : QWidget( ( layout && layout->parentWidget() ) ? layout->parentWidget() : 0 )
    , symbolsFactory_( symbolsFactory )
{
    QWidget* group = new QWidget( this, "group" );
    layout->addWidget( group, 0, 0, 1, 2 );
    QVBoxLayout* boxLayout = new QVBoxLayout();
    group->setLayout( boxLayout );
    buttonsLayout_ = new QGridLayout();
    QWidget* buttonsGroup = new QWidget( this, "buttonsGroup" );
    buttonsGroup->setLayout( buttonsLayout_ );
    boxLayout->addWidget( buttonsGroup );
    if( stretch )
        boxLayout->addWidget( new QWidget(), stretch );

    // Level
    buttonsLayout_->addWidget( new QLabel( tr( "Hierarchy level:" ), this ), row, 0 );
    levelComboBox_ = new gui::RichWidget< QComboBox >( "levelComboBox", this );
    levelBase_ = tr( "Select a parent automat or formation" );
    connect( levelComboBox_, SIGNAL( activated( int ) ), SLOT( UpdateSymbol() ) );
    buttonsLayout_->addWidget( levelComboBox_, row, 1 );

    // Nature
    natureWidget_ = new gui::NatureEditionWidget( buttonsLayout_, row + 1, 4 );
    connect( natureWidget_, SIGNAL( textChanged( const QString& ) ), SLOT( UpdateSymbol() ) );

    // Icon
    icon_ = new gui::UnitPreviewIcon( this, controllers, icons, colorStrategy, iconPreviewToolTips );
    boxLayout->addWidget( icon_,0, Qt::AlignBottom );
    connect( icon_, SIGNAL( StartDrag() ), SIGNAL( StartDrag() ) );
    connect( icon_, SIGNAL( SelectionChanged( const kernel::Entity_ABC* ) ), SLOT( OnSelectionChanged( const kernel::Entity_ABC* ) ) );

    // Configuration
    for( unsigned int i = 0; i < unsigned int( eNbrNatureLevel ); ++i )
        levelNames_ << tools::ToString( static_cast< E_NatureLevel >( i ), ENT_Tr::eToSim );
    OnSelectionChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor destructor
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
EntitySymbolEditor::~EntitySymbolEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::GetSymbol
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
const std::string& EntitySymbolEditor::GetSymbol() const
{
    return icon_->GetSymbol();
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::GetNature
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
const std::string& EntitySymbolEditor::GetNature() const
{
    return nature_;
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::GetLevel
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
const std::string& EntitySymbolEditor::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::GetGhostType
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
E_GhostType EntitySymbolEditor::GetGhostType() const
{
    return ghostType_;
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::IsLevelValid
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
bool EntitySymbolEditor::IsLevelValid() const
{
    return levelComboBox_->currentText() != levelBase_;
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::UpdateSymbol
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void EntitySymbolEditor::UpdateSymbol()
{
    nature_ = natureWidget_->text().toStdString();
    icon_->SetSymbol( symbolsFactory_.CreateSymbol( nature_ ) );
    if( levelComboBox_->currentText() != levelBase_ )
    {
        level_ = levelComboBox_->currentText().toStdString();
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
// Name: EntitySymbolEditor::FillLevel
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
void EntitySymbolEditor::FillLevel()
{
    levelComboBox_->clear();
    for( int i = eNatureLevel_xxxxx; i > eNatureLevel_None; --i )
        levelComboBox_->addItem( levelNames_[ i ] );
    emit( LevelChanged() );
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::OnSelectionChanged
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void EntitySymbolEditor::OnSelectionChanged( const kernel::Entity_ABC* parent )
{
    ghostType_ = eGhostType_Invalid;
    levelComboBox_->clear();
    if( parent )
    {
        ghostType_ = ( parent->GetTypeName() == kernel::Automat_ABC::typeName_ ) ? eGhostType_Agent :
            ( parent->GetTypeName() == kernel::Formation_ABC::typeName_ ) ? eGhostType_Automat :
            eGhostType_Invalid;
    if( ghostType_ != eGhostType_Invalid )
    {
        FillLevel();
        std::string level = parent->Get< kernel::TacticalHierarchies >().GetLevel();
        level.erase( 0, 7 );
        levelComboBox_->setCurrentText( level.c_str() );
    }
    else
        levelComboBox_->addItem( levelBase_ );
    }
    else
        levelComboBox_->addItem( levelBase_ );
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::Reset
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
void EntitySymbolEditor::Reset()
{
    natureWidget_->Clear();
    natureWidget_->SetRootSymbolRule( *symbolsFactory_.GetSymbolRule() );
    natureWidget_->setText( "undefined/undefined/undefined/undefined" );
    OnSelectionChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::Fill
// Created: ABR 2011-11-02
// -----------------------------------------------------------------------------
void EntitySymbolEditor::Fill( const kernel::Entity_ABC& entity, const QString& nature, const QString& level )
{
    natureWidget_->Clear();
    natureWidget_->SetRootSymbolRule( *symbolsFactory_.GetSymbolRule() );
    natureWidget_->setText( "undefined/undefined/undefined/undefined" );
    FillLevel();
    levelComboBox_->setCurrentText( level );
    icon_->SetSelectedParent( &entity );
    UpdateSymbol();
    natureWidget_->setText( nature );
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::InsertWidget
// Created: LDC 2014-05-27
// -----------------------------------------------------------------------------
void EntitySymbolEditor::InsertWidget( QWidget* widget, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment )
{
    buttonsLayout_->addWidget( widget, row, column, rowSpan, columnSpan, alignment );
}

// -----------------------------------------------------------------------------
// Name: EntitySymbolEditor::SetDeep
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
void EntitySymbolEditor::SetDeep( int deep )
{
    natureWidget_->SetDeep( deep );
}
