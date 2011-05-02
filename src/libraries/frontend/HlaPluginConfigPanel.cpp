// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "HlaPluginConfigPanel.h"
#include "CreateSession.h"
#include "tools/GeneralConfig.h"
#include "clients_gui/Tools.h"
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qspinbox.h>

using namespace frontend;

namespace
{
    template< typename T >
    T* Style( T* widget )
    {
        widget->setBackgroundOrigin( QWidget::WindowOrigin );
        return widget;
    }
}

// -----------------------------------------------------------------------------
// Name: HlaPluginConfigPanel constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
HlaPluginConfigPanel::HlaPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_ ( config )
    , library_( "hla_plugin-vc80-mt.dll" ) // $$$$ SLI 2011-05-02: hard coded library name
{
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    box_ = Style( new QGroupBox( 2, Horizontal, tools::translate( "HlaPluginConfigPanel", "Enable HLA RPR-FOM export" ), this ) );
    box_->setCheckable( true );
    box_->setChecked( false );
    {
        Style( new QLabel( tools::translate( "HlaPluginConfigPanel", "Federation: " ), box_ ) );
        federation_ = Style( new QLineEdit( "MyFederation", box_ ) );
    }
    {
        Style( new QLabel( tools::translate( "HlaPluginConfigPanel", "Federate name: " ), box_ ) );
        name_ = Style( new QLineEdit( tools::translate( "Application", "SWORD" ), box_ ) );
    }
    {
        Style( new QLabel( tools::translate( "HlaPluginConfigPanel", "Host name: " ), box_ ) );
        host_ = Style( new QLineEdit( "localhost", box_ ) );
    }
    {
        Style( new QLabel( tools::translate( "HlaPluginConfigPanel", "Port: " ), box_ ) );
        port_ = Style( new QSpinBox( 0, 65535, 1, box_ ) );
        port_->setValue( 8989 );
    }
}

// -----------------------------------------------------------------------------
// Name: HlaPluginConfigPanel destructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
HlaPluginConfigPanel::~HlaPluginConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaPluginConfigPanel::Commit
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void HlaPluginConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    if( box_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetOption( "session/config/dispatcher/plugins/hla/@library", library_ );
        action.SetOption( "session/config/dispatcher/plugins/hla/@federation", federation_->text() );
        action.SetOption( "session/config/dispatcher/plugins/hla/@name", name_->text() );
        action.SetOption( "session/config/dispatcher/plugins/hla/@host", host_->text() );
        action.SetOption( "session/config/dispatcher/plugins/hla/@port", port_->value() );
        action.Commit();
    }
}
