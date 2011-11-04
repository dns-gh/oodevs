// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "GhostsPanel.h"
#include "moc_GhostsPanel.cpp"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/GhostPrototype.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolRule.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/UnitPreviewIcon.h"
#include "clients_gui/ValuedDragObject.h"

#include "GhostSymbolEditor.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: GhostsPanel constructor
// Created: ABR 2011-10-12
// -----------------------------------------------------------------------------
GhostsPanel::GhostsPanel( QWidget* parent, gui::PanelStack_ABC& panel, Controllers& controllers, SymbolFactory& symbolsFactory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy )
    : InfoPanel_ABC( parent, panel, tr( "Phantom" ), "GhostsPanel" )
    , controllers_        ( controllers )
    , symbolsFactory_     ( symbolsFactory )
{
    // Base
    QWidget* box = new QWidget( this );
    QGridLayout* layout = new QGridLayout( box );
    layout->setAlignment( Qt::AlignTop );
    layout->setMargin( 10 );
    layout->setSpacing( 10 );
    setWidget( box );
    // Name
    layout->addWidget( new QLabel( tr( "Name:" ), this ), 0, 0 );
    nameLineEdit_ = new QLineEdit( this );
    connect( nameLineEdit_, SIGNAL( textChanged( const QString& ) ), SLOT( UpdateWarning() ) );
    layout->addWidget( nameLineEdit_, 0, 1 );
    // Type
    layout->addWidget( new QLabel( tr( "Type:" ), this ), 1, 0 );
    typeLineEdit_ = new QLineEdit( this );
    connect( typeLineEdit_, SIGNAL( textChanged( const QString& ) ), SLOT( UpdateWarning() ) );
    layout->addWidget( typeLineEdit_, 1, 1 );

    // Symbol Editor
    symbolEditor_ = new GhostSymbolEditor( layout, 2, controllers_, symbolsFactory_, icons, colorStrategy );
    connect( symbolEditor_, SIGNAL( LevelChanged() ), SLOT( UpdateWarning() ) );
    connect( symbolEditor_, SIGNAL( StartDrag() ), SLOT( IconDragged() ) );

    // Warning
    warningLabel_ = new QLabel( this );
    layout->addWidget( warningLabel_, 10, 0, 1, 2, Qt::AlignCenter );

    // Configuration
    controllers_.Register( *this );

    UpdateWarning();
}

// -----------------------------------------------------------------------------
// Name: GhostsPanel destructor
// Created: ABR 2011-10-12
// -----------------------------------------------------------------------------
GhostsPanel::~GhostsPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::NotifyUpdated
// Created: ABR 2011-10-12
// -----------------------------------------------------------------------------
void GhostsPanel::NotifyUpdated( const ModelLoaded& )
{
    nameLineEdit_->clear();
    typeLineEdit_->clear();
    nameLineEdit_->setPlaceholderText( tr( "Enter phantom name here" ) );
    typeLineEdit_->setPlaceholderText( tr( "Enter phantom type here" ) );
    Show();
}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::Load
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
void GhostsPanel::Load()
{
    symbolEditor_->Reset();
}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::IconDragged
// Created: ABR 2011-10-17
// -----------------------------------------------------------------------------
void GhostsPanel::IconDragged()
{
    protoType_.name_ = nameLineEdit_->text().ascii();
    protoType_.type_ = typeLineEdit_->text().ascii();
    protoType_.ghostType_ = symbolEditor_->GetGhostType();
    protoType_.symbol_ = symbolEditor_->GetSymbol();
    protoType_.level_ = symbolEditor_->GetLevel();
    if( warningLabel_->text().isEmpty() )
    {
        Q3DragObject* drag = new gui::ValuedDragObject( &protoType_, this );
        drag->drag();
    }
}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::UpdateWarning
// Created: ABR 2011-10-17
// -----------------------------------------------------------------------------
void GhostsPanel::UpdateWarning()
{
    QString error;
    if( nameLineEdit_->text().isEmpty() )
        error += tr( "- Name not set.<br>" );
    if( typeLineEdit_->text().isEmpty() )
        error += tr( "- Type not set.<br>" );
    if( !symbolEditor_->IsLevelValid() )
        error += tr( "- Superior not set to a formation or an automat.<br>" );
    if( !error.isEmpty() )
        error = "<font color=\"#FF0000\">" + tr( "Error, missing informations:<br>" ) + error + "</font>";
    warningLabel_->setText( error );
}


//// -----------------------------------------------------------------------------
//// Name: GhostsPanel::ResetCombo
//// Created: ABR 2011-10-17
//// -----------------------------------------------------------------------------
//void GhostsPanel::ResetCombo( QComboBox& combo, GhostsPanel::T_TraductionMap& traductionMap )
//{
//    combo.clear();
//    traductionMap.clear();
//    traductionMap[ tools::findTranslation( "models::app6", "undefined" ) ] = "undefined";
//}
//
//// -----------------------------------------------------------------------------
//// Name: GhostsPanel::FillCombo
//// Created: ABR 2011-10-17
//// -----------------------------------------------------------------------------
//void GhostsPanel::FillCombo( QComboBox& combo, GhostsPanel::T_TraductionMap& traductionMap )
//{
//    for( CIT_TraductionMap it = traductionMap.begin(); it != traductionMap.end(); ++it )
//        combo.insertItem( it->first );
//    Select( combo, "undefined" );
//}
//
//// -----------------------------------------------------------------------------
//// Name: GhostsPanel::Select
//// Created: ABR 2011-10-17
//// -----------------------------------------------------------------------------
//void GhostsPanel::Select( QComboBox& combo, const QString& value )
//{
//    QString translatedValue = tools::findTranslation( "models::app6",  value );
//    for( int i = 0; i < combo.count(); ++i )
//        if( translatedValue == combo.text( i ) )
//        {
//            combo.setCurrentItem( i );
//            return;
//        }
//}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::OnCategoryChange
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
//void GhostsPanel::OnCategoryChange()
//{
//    assert( categoryRule_ );
//    const QString current = categoryComboBox_->currentText();
//    if( tools::findTranslation( "models::app6", categoryCurrentName_ ) != current )
//    {
//        categoryCurrentName_ = categoryNames_[ current ].c_str();
//        status_ = eSwapping;
//        categoryRule_->Accept( *this );
//        UpdateSymbol();
//    }
//}
//
//// -----------------------------------------------------------------------------
//// Name: GhostsPanel::StartCategory
//// Created: ABR 2011-10-14
//// -----------------------------------------------------------------------------
//void GhostsPanel::StartCategory( const std::string& /*title*/ )
//{
//    if( status_ == eSwapping || status_ == eVisitingWeapon )
//        ResetCombo( *weaponComboBox_, weaponNames_ );
//}
//
//// -----------------------------------------------------------------------------
//// Name: GhostsPanel::AddChoice
//// Created: ABR 2011-10-17
//// -----------------------------------------------------------------------------
//void GhostsPanel::AddChoice( const std::string& name, GhostsPanel::T_TraductionMap& traductionMap )
//{
//    QString translatedName = tools::findTranslation( "models::app6", name.c_str() );
//    traductionMap[ translatedName ] = name;
//}
//
//// -----------------------------------------------------------------------------
//// Name: GhostsPanel::AddChoice
//// Created: ABR 2011-10-14
//// -----------------------------------------------------------------------------
//void GhostsPanel::AddChoice( SymbolRule* rule, const std::string& name, const std::string& /*value*/ )
//{
//    if( status_ == eLoading )
//        AddChoice( name, categoryNames_ );
//    else if( status_ == eSwapping && rule && name == categoryCurrentName_.ascii() )
//    {
//        status_ = eVisitingWeapon;
//        rule->Accept( *this );
//        status_ = eSwapping;
//    }
//    else if( status_ == eVisitingWeapon && categoryComboBox_->currentIndex() != categoryComboBox_->count() - 1 )
//        AddChoice( name, weaponNames_ );
//}
//
//// -----------------------------------------------------------------------------
//// Name: GhostsPanel::EndCategory
//// Created: ABR 2011-10-14
//// -----------------------------------------------------------------------------
//void GhostsPanel::EndCategory()
//{
//    if( status_ == eLoading )
//    {
//        FillCombo( *categoryComboBox_, categoryNames_ );
//        FillCombo( *weaponComboBox_, weaponNames_ );
//    }
//    else if( status_ == eVisitingWeapon )
//        FillCombo( *weaponComboBox_, weaponNames_ );
//    if( weaponComboBox_->count() == 0 )
//        FillCombo( *weaponComboBox_, weaponNames_ );
//}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::OnSelectionChanged
// Created: ABR 2011-10-17
// -----------------------------------------------------------------------------
//void GhostsPanel::OnSelectionChanged( const Entity_ABC* parent )
//{
//    E_NatureLevel parentLevel = eNatureLevel_None;
//    protoType_.ghostType_ = eGhostType_Invalid;
//    if( parent )
//        protoType_.ghostType_ = ( parent->GetTypeName() == Automat_ABC::typeName_ ) ? eGhostType_Agent :
//                                ( parent->GetTypeName() == Formation_ABC::typeName_ ) ? eGhostType_Automat :
//                                eGhostType_Invalid;
//
//    if( protoType_.ghostType_ != eGhostType_Invalid )
//    {
//        const TacticalHierarchies* pHierarchy = parent->Retrieve< TacticalHierarchies >();
//        if( pHierarchy )
//        {
//            std::string level = pHierarchy->GetLevel();
//            level.erase( 0, 7 );
//            parentLevel = tools::NatureLevelFromString( level );
//        }
//    }
//    levelComboBox_->clear();
//    for( int i = parentLevel - 1; i > eNatureLevel_None; --i )
//        levelComboBox_->addItem( levelNames_[ i ] );
//    if( levelComboBox_->count() == 0 )
//        levelComboBox_->addItem( levelBase_ );
//    UpdateSymbol();
//    UpdateWarning();
//}

// -----------------------------------------------------------------------------
// Name: GhostsPanel::UpdateSymbol
// Created: ABR 2011-10-17
// -----------------------------------------------------------------------------
//void GhostsPanel::UpdateSymbol()
//{
//    std::string symbol = std::string( categoryCurrentName_.ascii() ) + "/" + weaponNames_[ weaponComboBox_->currentText() ] + "/undefined/undefined";
//    symbol = symbolsFactory_.CreateSymbol( symbol );
//    icon_->SetSymbol( symbol );
//    if( levelComboBox_->currentText() != levelBase_ )
//        icon_->SetLevel( std::string( "levels/" ) + levelComboBox_->currentText().ascii() );
//    else
//        icon_->SetLevel( "" );
//    icon_->UpdateSymbol();
//}
