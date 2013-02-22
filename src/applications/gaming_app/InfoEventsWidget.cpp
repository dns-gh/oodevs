// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoEventsWidget.h"
#include "gaming/Attributes.h"
#include "gaming/Contaminations.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/NBCAgent.h"
#include "tools/GeneralConfig.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget constructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoEventsWidget::InfoEventsWidget( QWidget* parent, kernel::Controllers& controllers )
    : Q3VBox( parent, "InfoEventsWidget" )
    , controllers_( controllers )
    , selected_( controllers )
{
    setFixedWidth( 27 );
    setMargin( 0 );
    InitializeEvents( this );
    layout()->setAlignment( Qt::AlignTop );
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget destructor
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
InfoEventsWidget::~InfoEventsWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::InitializeEvents
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void InfoEventsWidget::InitializeEvents( QWidget* parent )
{
    const QPixmap defaultIcon( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() );

    events_["jammed"]         = CreateEvent( parent, MAKE_PIXMAP( brouillage ),
                                    tools::translate( "InfoEventsWidget", "Communication: jammed" ) );
    events_["silenceEmitted"] = CreateEvent( parent, MAKE_PIXMAP( silence_radio_outgoing ),
                                    tools::translate( "InfoEventsWidget", "Communication: radio emitter silence" ) );
    events_["silenceReceived"]= CreateEvent( parent, MAKE_PIXMAP( silence_radio_incoming ),
                                    tools::translate( "InfoEventsWidget", "Communication: radio receiver silence" ) );
    events_["radar"]          = CreateEvent( parent, MAKE_PIXMAP( radars_on ),
                                    tools::translate( "InfoEventsWidget", "Communication: radar enabled" ) );
    events_["stealth"]        = CreateEvent( parent, defaultIcon,
                                    tools::translate( "InfoEventsWidget", "Communication: stealth mode" ) );
    events_["nbc suit"]       = CreateEvent( parent, QPixmap( "resources/images/gaming/nbcSuit.png" ),
                                    tools::translate( "InfoEventsWidget", "NBC: suit on" ) );
    events_["underground"]    = CreateEvent( parent, MAKE_PIXMAP( underground ),
                                    tools::translate( "InfoEventsWidget", "Underground" ) );
    events_["contamination"]  = CreateEvent( parent, QPixmap( "resources/images/gaming/nbc.png" ), "" );
    events_["refugees"]       = CreateEvent( parent, defaultIcon,
                                    tools::translate( "InfoEventsWidget", "Refugees handled" ) );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::CreateEvent
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
EventWidget* InfoEventsWidget::CreateEvent( QWidget* parent, const QPixmap& pixmap, const QString& text )
{
    return new EventWidget( parent, pixmap, text );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::ToggleEvent
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
void InfoEventsWidget::ToggleEvent( const std::string& event, bool toggle )
{
    events_[event]->setVisible( toggle );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::SetAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void InfoEventsWidget::SetAttributes( const Attributes& attributes )
{
    ToggleEvent( "jammed"  , attributes.bCommJammed_ );
    ToggleEvent( "silenceEmitted" , attributes.bRadioEmitterSilence_ );
    ToggleEvent( "silenceReceived" , attributes.bRadioReceiverSilence_ );
    ToggleEvent( "radar"   , attributes.bRadarEnabled_ );
    ToggleEvent( "stealth" , attributes.bStealthModeEnabled_ );
    ToggleEvent( "refugees", attributes.bRefugeesManaged_ );
    ToggleEvent( "underground", attributes.bUnderground_ );
    SetShown();
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::SetContaminations
// Created: SBO 2007-02-09
// -----------------------------------------------------------------------------
void InfoEventsWidget::SetContaminations( const Contaminations& attributes )
{
    ToggleEvent( "nbc suit", attributes.bNbcProtectionSuitWorn_ );
    if( attributes.type_ != eAgentNone )
    {
        EventWidget* label = events_["nbc suit"];
        label->setText( locale().toString( attributes.type_ ) );
    }
    ToggleEvent( "contamination", !attributes.contaminatingNbcAgents_.empty() || attributes.contaminated_ );
    if( !attributes.contaminatingNbcAgents_.empty() )
    {
        EventWidget* label = events_["contamination"];
        QStringList agents;
        for( unsigned int i = 0; i < attributes.contaminatingNbcAgents_.size(); ++i )
            agents.append( attributes.contaminatingNbcAgents_[i]->GetName().c_str() );
        label->setToolTip( tools::translate( "InfoEventsWidget", "NBC: contamination of type '%1'" ).arg( agents.join( ", " ) ) );
    }
    SetShown();
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::NotifySelected
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
void InfoEventsWidget::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( selected_ != entity )
    {
        selected_ = entity;
        hide();
        Reset();
        if( selected_ )
        {
            if( const Attributes* attributes = selected_->Retrieve< Attributes >() )
                SetAttributes( *attributes );
            if( const Contaminations* contaminations = selected_->Retrieve< Contaminations >() )
                SetContaminations( *contaminations );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::NotifyUpdated
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void InfoEventsWidget::NotifyUpdated( const Attributes& element )
{
    if( !selected_ || selected_->Retrieve< Attributes >() != &element )
        return;
    SetAttributes( element );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::NotifyUpdated
// Created: SBO 2007-02-09
// -----------------------------------------------------------------------------
void InfoEventsWidget::NotifyUpdated( const Contaminations& element )
{
    if( !selected_ )
        return;
    const Contaminations* contaminations = selected_->Retrieve< Contaminations >();
    if( contaminations == &element )
        SetContaminations( element );
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::SetShown
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void InfoEventsWidget::SetShown()
{
    for( T_Events::const_iterator it = events_.begin(); it != events_.end(); ++it )
        if( it->second->isShown() )
        {
            show();
            return;
        }
    hide();
}

// -----------------------------------------------------------------------------
// Name: InfoEventsWidget::Reset
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void InfoEventsWidget::Reset()
{
    for( T_Events::const_iterator it = events_.begin(); it != events_.end(); ++it )
        it->second->hide();
}
