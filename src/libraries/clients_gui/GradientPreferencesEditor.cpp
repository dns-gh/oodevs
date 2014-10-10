// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GradientPreferencesEditor.h"
#include "moc_GradientPreferencesEditor.cpp"

#include "ColorButton.h"
#include "Gradient.h"
#include "GradientPreferences.h"
#include "GradientWidget.h"
#include "OptionWidgets.h"
#include "PresetDialog.h"
#include "resources.h"
#include "RichWidget.h"
#include "SubObjectName.h"
#include "Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Tools.h"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

Q_DECLARE_METATYPE( gui::GradientPreferencesEditor::T_Gradient );

using namespace gui;

namespace
{
    QToolButton* CreateButton( const QString& objectName,
                               const QString& textLabel,
                               const QPixmap& pixmap,
                               const QObject* receiver,
                               const char* slot )
    {
        QToolButton* button = new RichWidget< QToolButton >( objectName );
        button->setPixmap( pixmap );
        button->setTextLabel( textLabel );
        button->setFixedSize( 22, 22 );
        QObject::connect( button, SIGNAL( clicked() ), receiver, slot );
        return button;
    }
}

// -----------------------------------------------------------------------------
// Name: GradientPreferencesEditor constructor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientPreferencesEditor::GradientPreferencesEditor( kernel::OptionsController& options,
                                                      const kernel::DetectionMap& detection,
                                                      const std::shared_ptr< GradientPreferences >& preferences,
                                                      const QString& gradientWidgetObjectName,
                                                      QWidget* parent /* = 0 */ )
    : QWidget( parent )
    , options_( options )
    , preferences_( preferences )
{
    SubObjectName subObject( "gradient-preferences-editor" );
    setMaximumHeight( 170 );

    auto fitToViewPort = new OptionCheckBox( options,
                                             "fitColorGradienttoViewPort",
                                             "Elevation/FitToViewPort",
                                             tr( "Fit color gradient to viewport") );

    combo_ = new RichWidget< QComboBox >( "presetCombo" );

    QHBoxLayout* toolsLayout = new QHBoxLayout();
    toolsLayout->setMargin( 0 );
    toolsLayout->addWidget( combo_, 1 );
    toolsLayout->addWidget( CreateButton( "copyPreset", tr( "Copy preset" ), MAKE_PIXMAP( copy ), this, SLOT( OnPresetCopied() ) ) );
    toolsLayout->addWidget( CreateButton( "renamePreset", tr( "Rename preset" ), MAKE_PIXMAP( pen_cursor ), this, SLOT( OnPresetRenamed() ) ) );
    toolsLayout->addWidget( CreateButton( "removePreset", tr( "Delete preset" ), MAKE_PIXMAP( trash ), this, SLOT( OnPresetDeleted() ) ) );

    const auto itemDrawer = [&detection]( QPainter& painter, unsigned int pourcentage, int x, int y ) {
        QFont font( "Normal", 8, QFont::Light );
        painter.setFont( font );
        const float elevation = detection.MaximumElevation() * pourcentage / 100.f;
        int position = pourcentage > 50 ? x - 5 : x;
        painter.drawText( position, y, boost::lexical_cast< std::string >( static_cast< unsigned int >( elevation ) ).c_str() );
    };
    gradientWidget_ = new GradientWidget( gradientWidgetObjectName, true, itemDrawer );

    QWidget* gradientWidget = new QWidget();
    QVBoxLayout* gradientLayout = new QVBoxLayout( gradientWidget );
    gradientLayout->setMargin( 0 );
    gradientLayout->addLayout( toolsLayout );
    gradientLayout->addWidget( gradientWidget_, 1 );

    QVBoxLayout* container = new QVBoxLayout( this );
    container->setMargin( 0 );
    container->addWidget( fitToViewPort );
    container->addWidget( tools::AddGroupBoxWidget( "gradient-groupbox", tr( "Gradient map" ), gradientWidget ) );
    container->addStretch();

    connect( fitToViewPort, SIGNAL( stateChanged( int ) ), SIGNAL( FitToViewportChanged( int ) ) );
    connect( fitToViewPort, SIGNAL( stateChanged( int ) ), gradientWidget_, SIGNAL( FitToViewPortChanged( int ) ) );
    connect( gradientWidget_, SIGNAL( GradientChanged() ), SLOT( OnGradientEdited() ) );
    connect( combo_, SIGNAL( activated( int ) ), SLOT( OnSelectionChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferencesEditor destructor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientPreferencesEditor::~GradientPreferencesEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GradientPreferencesEditor::Load
// Created: ABR 2014-10-06
// -----------------------------------------------------------------------------
void GradientPreferencesEditor::Load()
{
    combo_->clear();
    preferences_->Apply( [&]( const T_Gradient& gradient ) {
        AddGradient( gradient );
    } );
    SetCurrent();
    gradientWidget_->LoadGradient( preferences_->GetCurrent() );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferencesEditor::AddGradient
// Created: ABR 2014-07-31
// -----------------------------------------------------------------------------
void GradientPreferencesEditor::AddGradient( const T_Gradient& gradient )
{
    const auto& name = gradient->GetDisplayName();
    combo_->insertItem( -1, name, QVariant::fromValue< T_Gradient >( gradient ) ); // in Qt::UserRole
    combo_->setCurrentIndex( combo_->findText( name ) );
    combo_->model()->sort( 0, Qt::AscendingOrder );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferencesEditor::OnGradientEdited
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferencesEditor::OnGradientEdited()
{
    auto current = preferences_->GetCurrent();
    options_.Change( "Elevation/Gradients/" + current->GetName().toStdString(), current->GetValues() );
    emit UpdateGradient();
}

// -----------------------------------------------------------------------------
// Name: GradientPreferencesEditor::OnPresetChanged
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferencesEditor::OnSelectionChanged()
{
    auto current = combo_->itemData( combo_->currentIndex(), Qt::UserRole ).value< T_Gradient >();
    preferences_->SetCurrent( current );
    options_.Change( "Elevation/Gradient", current->GetName(), true );
    gradientWidget_->LoadGradient( current );
    emit UpdateGradient();
}

// -----------------------------------------------------------------------------
// Name: GradientPreferencesEditor::OnPresetCopied
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferencesEditor::OnPresetCopied()
{
    auto current = preferences_->GetCurrent();
    unsigned int i = 1;
    QString newName;
    bool valid;
    do {
        valid = true;
        newName = QString( "%1 (%L2)" ).arg( current->GetDisplayName() ).arg( ++i );
        preferences_->Apply( [&]( const T_Gradient& gradient ) {
            if( gradient->GetName() == newName )
                valid = false;
        } );
    } while( !valid );
    T_Gradient copy = std::make_shared< Gradient >( *current );
    copy->SetName( newName );
    AddGradient( copy );
    preferences_->Add( copy );
    preferences_->SetCurrent( copy );
    SetCurrent();
    options_.Change( "Elevation/Gradients/" + newName.toStdString(), copy->GetValues() );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferencesEditor::OnPresetRenamed
// Created: ABR 2014-10-09
// -----------------------------------------------------------------------------
void GradientPreferencesEditor::OnPresetRenamed()
{
    auto current = preferences_->GetCurrent();
    PresetDialog* pDialog = new PresetDialog( this, options_, current, *preferences_ );
    if( pDialog->exec() == QDialog::Accepted )
        Load();
}

// -----------------------------------------------------------------------------
// Name: GradientPreferencesEditor::OnPresetDeleted
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferencesEditor::OnPresetDeleted()
{
    if( preferences_->Count() == 1 )
        return;
    auto name = combo_->currentText();
    combo_->removeItem( combo_->currentItem() );
    options_.Remove( "Elevation/Gradients/" + name.toStdString() );
    preferences_->Remove( preferences_->GetCurrent() );
    combo_->setCurrentItem( 0 );
    OnSelectionChanged();
}

// -----------------------------------------------------------------------------
// Name: GradientPreferencesEditor::SetCurrent
// Created: ABR 2014-07-28
// -----------------------------------------------------------------------------
void GradientPreferencesEditor::SetCurrent()
{
    auto current = preferences_->GetCurrent();
    int index = combo_->findText( current->GetDisplayName() );
    combo_->setCurrentIndex( index < 0 ? 0 : index );
}
