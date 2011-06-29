// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::IntelligencesPanel */

#include "clients_gui_pch.h"
#include "IntelligencesPanel.h"
#include "moc_IntelligencesPanel.cpp"
#include "NatureSelectionWidget.h"
#include "ValuedComboBox.h"
#include "SymbolIcons.h"
#include "ValuedDragObject.h"
#include "clients_kernel/IntelligencePrototype.h"
#include "tools/Iterator.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/Tools.h"
#include "tools/ExerciseConfig.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel constructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
IntelligencesPanel::IntelligencesPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, const FormationLevels& levels, SymbolIcons& icons, const tools::ExerciseConfig* config /*= 0*/ )
    : InfoPanel_ABC( parent, panel, tr( "Intelligence" ), "IntelligencesPanel" )
    , controllers_   ( controllers )
    , icons_         ( icons )
    , symbolFactory_ ( new SymbolFactory() )
    , selectedEntity_( controllers )
{
    if( config )
        symbolFactory_->Load( *config );
    layout()->setAlignment( Qt::AlignTop );
    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, tr( "Intelligence description" ), this );
    {
        new QLabel( tr( "Superior:" ), group );
        superiorLabel_ = new QLabel( "---", group );
    }
    {
        new QLabel( tr( "Level: " ), group );
        levelCombo_ = new ValuedComboBox< const HierarchyLevel_ABC* >( group );
        tools::Iterator< const HierarchyLevel_ABC& > it( levels.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const HierarchyLevel_ABC& level = it.NextElement();
            levelCombo_->AddItem( tools::findTranslation( "models::app6", level.GetName() ), &level );
        }
        connect( levelCombo_, SIGNAL( activated( int ) ), SLOT( UpdateSymbol() ) );
    }
    {
        new QLabel( tr( "Karma: " ), group );
        karmaCombo_ = new QComboBox( group );
        karmaCombo_->insertItem( Karma::friend_ .GetName() );
        karmaCombo_->insertItem( Karma::enemy_  .GetName() );
        karmaCombo_->insertItem( Karma::neutral_.GetName() );
        karmaCombo_->insertItem( Karma::unknown_.GetName() );
        connect( karmaCombo_, SIGNAL( activated( int ) ), SLOT( UpdateSymbol() ) );
    }
    {
        new QLabel( tr( "Mounted combat: " ), group );
        mounted_ = new QCheckBox( group );
    }
    if( config )
    {
        NatureSelectionWidget* nature = new NatureSelectionWidget( this, config->GetLoader() );
        connect( nature, SIGNAL( NatureSelected( const QString& ) ), SLOT( OnNatureChanged( const QString& ) ) );
        connect( nature, SIGNAL( StartDrag() ), SLOT( OnNatureDragged() ) );
    }
    {
        QHBox* box = new QHBox( this );
        box->setFrameStyle( QFrame::Plain | QFrame::Box );
        box->setPaletteBackgroundColor( Qt::white );
        box->layout()->setAlignment( Qt::AlignCenter | Qt::AlignHCenter );
        icon_ = new QLabel( box );
        icon_->setMargin( 10 );
        QToolTip::add( icon_, tr( "Drag and drop symbol to map to create a new intelligence unit." ) );
        UpdateSymbol();
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel destructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
IntelligencesPanel::~IntelligencesPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel::NotifyUpdated
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void IntelligencesPanel::NotifyUpdated( const ModelLoaded& )
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel::BeforeSelection
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void IntelligencesPanel::BeforeSelection()
{
    selectedEntity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel::AfterSelection
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void IntelligencesPanel::AfterSelection()
{
    superiorLabel_->setText( selectedEntity_ ? selectedEntity_->GetName() : "---" );
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel::Select
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void IntelligencesPanel::Select( const Formation_ABC& element )
{
    selectedEntity_ = &element;
}

namespace
{
    QColor GetColor( const Karma& karma )
    {
        if( karma == Karma::friend_ )
            return QColor( 100, 125, 255 );
        if( karma == Karma::enemy_ )
            return QColor( 255, 50, 50 );
        if( karma == Karma::neutral_ )
            return QColor( 0, 170, 0 );
        return QColor( 255, 220, 000 );
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel::UpdateSymbol
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void IntelligencesPanel::UpdateSymbol()
{
    if( !nature_.isEmpty() )
    {
        symbol_ = symbolFactory_->CreateSymbol( nature_.ascii() );
        const Karma& karma = Karma::ResolveName( karmaCombo_->currentText() );
        App6Symbol::SetKarma( symbol_, karma );
        SymbolIcon icon( symbol_, symbolFactory_->CreateLevelSymbol( *levelCombo_->GetValue() ) );
        icon.SetColor( GetColor( karma ) );
        icon.SetSize( 128 );
        QPixmap pixmap = icons_.GetSymbol( icon );
        if( !pixmap.isNull() )
            icon_->setPixmap( pixmap );
        else
            QTimer::singleShot( 100, this, SLOT( UpdateSymbol() ) );
    }
    else
    {
        QPixmap blank( 128, 128 );
        blank.fill( Qt::white );
        icon_->setPixmap( blank );
    }
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel::OnNatureChanged
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void IntelligencesPanel::OnNatureChanged( const QString& text )
{
    nature_ = text;
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel::OnNatureDragged
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void IntelligencesPanel::OnNatureDragged()
{
    DoDrag();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel::mouseMoveEvent
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
void IntelligencesPanel::mouseMoveEvent( QMouseEvent* event )
{
    if( event->button() | Qt::LeftButton )
        DoDrag();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel::DoDrag
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void IntelligencesPanel::DoDrag()
{
    if( !selectedEntity_ )
        return;
    intelligence_.reset( new IntelligencePrototype( *selectedEntity_.ConstCast(), symbol_, *levelCombo_->GetValue(), mounted_->isChecked(), Karma::ResolveName( karmaCombo_->currentText() ) ) );
    QDragObject* drag = new ValuedDragObject( intelligence_.get(), this );
    drag->drag();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel::Load
// Created: ABR 2011-05-25
// -----------------------------------------------------------------------------
void IntelligencesPanel::Load( const tools::ExerciseConfig& config )
{
    symbolFactory_->Load( config );

    NatureSelectionWidget* nature = new NatureSelectionWidget( this, config.GetLoader() );
    connect( nature, SIGNAL( NatureSelected( const QString& ) ), SLOT( OnNatureChanged( const QString& ) ) );
    connect( nature, SIGNAL( StartDrag() ), SLOT( OnNatureDragged() ) );
}
