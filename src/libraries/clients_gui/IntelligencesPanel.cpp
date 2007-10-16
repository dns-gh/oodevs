// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "IntelligencesPanel.h"
#include "moc_IntelligencesPanel.cpp"
#include "NatureSelectionWidget.h"
#include "ValuedComboBox.h"
#include "SymbolIcons.h"
#include "ValuedDragObject.h"
#include "clients_kernel/IntelligencePrototype.h"
#include "clients_kernel/Iterator.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Team_ABC.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IntelligencesPanel constructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
IntelligencesPanel::IntelligencesPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, const FormationLevels& levels, SymbolIcons& icons )
    : InfoPanel_ABC( parent, panel, tr( "Intelligence" ) )
    , controllers_( controllers )
    , icons_( icons )
    , symbolFactory_( new SymbolFactory() )
    , selectedTeam_( controllers )
{
    layout()->setAlignment( Qt::AlignTop );
    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, tr( "Intelligence description" ), this );
    {
        new QLabel( tr( "Owner team:" ), group );
        teamLabel_ = new QLabel( "---", group );
    }
    {
        new QLabel( tr( "Level: " ), group );
        levelCombo_ = new ValuedComboBox< const HierarchyLevel_ABC* >( group );
        Iterator< const HierarchyLevel_ABC& > it( levels.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const HierarchyLevel_ABC& level = it.NextElement();
            levelCombo_->AddItem( level.GetName(), &level );
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
        NatureSelectionWidget* nature = new NatureSelectionWidget( this, "symbols.xml" );
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
// Name: IntelligencesPanel::NotifySelected
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void IntelligencesPanel::NotifySelected( const kernel::Team_ABC* element )
{
    selectedTeam_ = element;
    teamLabel_->setText( selectedTeam_ ? selectedTeam_->GetName() : "---" );
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
        const std::string level  = symbolFactory_->CreateLevelSymbol( levelCombo_->GetValue()->GetName().ascii() );
        symbol_ = symbolFactory_->CreateSymbol( nature_.ascii() );
        const Karma& karma = Karma::ResolveName( karmaCombo_->currentText() );
        App6Symbol::SetKarma( symbol_, karma );
        QImage img;
        img = icons_.GetSymbol( symbol_, level, GetColor( karma ), QSize( 128, 128 ) ); // $$$$ SBO 2007-10-12: 
        if( !img.isNull() )
            icon_->setPixmap( img );
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
    if( !selectedTeam_ )
        return;
    intelligence_.reset( new IntelligencePrototype( *selectedTeam_.ConstCast(), symbol_, levelCombo_->GetValue()->GetSymbol(), Karma::ResolveName( karmaCombo_->currentText() ) ) );
    QDragObject* drag = new ValuedDragObject( intelligence_.get(), this );
    drag->drag();
}
