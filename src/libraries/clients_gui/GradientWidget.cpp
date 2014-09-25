// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GradientWidget.h"
#include "moc_GradientWidget.cpp"

#include "ColorButton.h"
#include "Gradient.h"
#include "GradientButton.h"
#include "GradientPreferences.h"
#include "PresetDialog.h"
#include "resources.h"
#include "RichWidget.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Tools.h"

#include <boost/bind.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GradientWidget constructor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientWidget::GradientWidget( QWidget* parent, GradientPreferences& preferences, kernel::Controllers& controllers,
                                const Painter_ABC& painter )
    : Q3VBox( parent )
    , controllers_( controllers )
    , options_    ( controllers_.options_ )
    , preferences_( preferences )
{
    setMaximumHeight( 150 );
    Q3HBox* box = new Q3HBox( this );
    presetCombo_ = new RichWidget< QComboBox >( "presetCombo", box );

    RichWidget< QToolButton >* copyPreset = new RichWidget< QToolButton >( "copyPreset", box );
    copyPreset->setPixmap( MAKE_PIXMAP( copy ) );
    copyPreset->setTextLabel( tr( "Copy preset" ) );
    copyPreset->setFixedSize( 22, 22 );

    RichWidget< QToolButton >* renamePreset = new RichWidget< QToolButton >( "renamePreset", box );
    renamePreset->setPixmap( MAKE_PIXMAP( pen_cursor ) );
    renamePreset->setTextLabel( tr( "Rename preset" ) );
    renamePreset->setFixedSize( 22, 22 );

    RichWidget< QToolButton >* removePreset = new RichWidget< QToolButton >( "removePreset", box );
    removePreset->setPixmap( MAKE_PIXMAP( trash ) );
    removePreset->setTextLabel( tr( "Delete preset" ) );
    removePreset->setFixedSize( 22, 22 );

    box = new Q3HBox( this );
    box->layout()->setAlignment( Qt::AlignCenter );
    box->setMaximumHeight( 100 );
    gradientEditor_ = new GradientButton( "gradient-editor", box, painter, true );
    color_ = new ColorButton( "color", box );
    color_->setMaximumHeight( 30 );

    connect( gradientEditor_, SIGNAL( SelectionChanged( const QColor& ) ), SLOT( OnSelectionChanged( const QColor& ) ) );
    connect( gradientEditor_, SIGNAL( GradientChanged( Gradient& ) ), SLOT( OnGradientEdited( Gradient& ) ) );
    connect( this, SIGNAL( ToggleVariableGradient( bool ) ), gradientEditor_, SLOT( OnEnableVariableGradient( bool ) ) );
    connect( color_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChanged( const QColor& ) ) );

    connect( presetCombo_, SIGNAL( activated( int ) ), SLOT( OnPresetChanged() ) );
    connect( copyPreset, SIGNAL( clicked() ), SLOT( OnPresetCopied() ) );
    connect( renamePreset, SIGNAL( clicked() ), SLOT( OnPresetRenamed() ) );
    connect( removePreset, SIGNAL( clicked() ), SLOT( OnPresetDeleted() ) );

    controllers_.Register( *this );
    Reset();
}

// -----------------------------------------------------------------------------
// Name: GradientWidget destructor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientWidget::~GradientWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::OnColorChanged
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientWidget::OnColorChanged( const QColor& color )
{
    gradientEditor_->SetSelectedColor( color );
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::OnSelectionChanged
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientWidget::OnSelectionChanged( const QColor& color )
{
    color_->SetColor( color );
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::OnGradientEdited
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientWidget::OnGradientEdited( Gradient& gradient )
{
    if( Gradient* current = CurrentPreset() )
        *current = gradient;
    emit GradientChanged( gradient );
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::OnPresetChanged
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientWidget::OnPresetChanged()
{
    if( const Gradient* current = CurrentPreset() )
        gradientEditor_->LoadGradient( *current );
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::OnPresetCopied
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientWidget::OnPresetCopied()
{
    if( const Gradient* current = CurrentPreset() )
    {
        Gradient* gradient = new Gradient( *current );
        unsigned int i = 1;
        QString newName;
        bool valid;
        do {
            valid = true;
            newName = QString( "%1 (%L2)" ).arg( current->GetName() ).arg( ++i );
            for( unsigned int i = 0; i < presets_.size(); ++i )
                if( presets_[i]->GetName() == newName )
                {
                    valid = false;
                    break;
                }
        } while( !valid );
        gradient->SetName( newName );
        presetCombo_->insertItem( newName );
        presets_.push_back( gradient );
    }
}

namespace
{
    void Update( Gradient* gradient, std::vector< std::string >& presets )
    {
        presets.push_back( gradient->GetName().toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::OnPresetRenamed
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
void GradientWidget::OnPresetRenamed()
{
    if( Gradient* current = CurrentPreset() )
    {
        std::vector< std::string > presets;
        std::for_each( presets_.begin(), presets_.end(), boost::bind( ::Update, _1, boost::ref( presets ) ) );
        PresetDialog* pDialog = new PresetDialog( this, *current, options_, presets );
        pDialog->exec();
        Commit();
    }
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::OnPresetDeleted
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientWidget::OnPresetDeleted()
{
    if( presets_.size() == 1 )
        return;
    if( Gradient* current = CurrentPreset() )
    {
        presetCombo_->removeItem( presetCombo_->currentItem() );
        options_.Remove( std::string( "Gradients/" ) + current->GetName().toStdString() );
        presets_.erase( std::find( presets_.begin(), presets_.end(), current ) );
        preferences_.Commit( presets_ );
        delete current;
        presetCombo_->setCurrentItem( 0 );
        OnPresetChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::CurrentPreset
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
Gradient* GradientWidget::CurrentPreset() const
{
    const int current = presetCombo_->currentItem();
    if( current < 0 || current > static_cast< int >( presets_.size() ) - 1 )
        return 0;
    return presets_.at( current );
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::Commit
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientWidget::Commit()
{
    if( const Gradient* current = CurrentPreset() )
        options_.Change( "Gradient", current->GetName() );
    preferences_.Commit( presets_ );
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::Reset
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientWidget::Reset()
{
    Clear();
    auto it = preferences_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Gradient& item = it.NextElement();
        presetCombo_->insertItem( tools::translate( "gradients",item.GetName() ) );
        presets_.push_back( new Gradient( item ) );
    }
    Select( options_.GetOption( "Gradient" ).To< QString >() );
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::Clear
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientWidget::Clear()
{
    presetCombo_->clear();
    for( auto it = presets_.begin(); it != presets_.end(); ++it )
        delete *it;
    presets_.clear();
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::OptionChanged
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientWidget::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "Gradient" )
        Select( value.To< QString >() );
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::Select
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientWidget::Select( const QString& presetName )
{
    for( unsigned int i = 0; i < presets_.size(); ++i )
        if( presets_[i]->GetName() == presetName )
        {
            presetCombo_->setCurrentItem( i );
            OnPresetChanged();
            break;
        }
}

// -----------------------------------------------------------------------------
// Name: GradientWidget::OnEnableVariableGradient
// Created: LGY 2010-09-29
// -----------------------------------------------------------------------------
void GradientWidget::OnEnableVariableGradient( bool state )
{
    emit ToggleVariableGradient( state );
}
