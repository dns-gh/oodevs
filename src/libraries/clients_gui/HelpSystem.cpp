// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "HelpSystem.h"
#include "moc_HelpSystem.cpp"

#include "clients_kernel/ModeController.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Tools.h"

#include "tools/GeneralConfig.h"
#include "tools/Language.h"

#include <xeumeuleu/xml.hpp>
#include <htmlhelp.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: HelpSystem constructor
// Created: AGE 2008-08-18
// -----------------------------------------------------------------------------
HelpSystem::HelpSystem( QWidget* root,
                        const tools::Path& config,
                        const kernel::ModeController* modeController /* = 0 */,
                        const kernel::Profile_ABC* profile /* = 0 */ )
    : QObject( root )
    , root_  ( root )
    , modeController_( modeController )
    , profile_( profile )
{
    try
    {
        tools::Xifstream xis( config );
        xis >> xml::start( "widgets" )
                >> xml::list( "widget", *this, &HelpSystem::ReadWidget )
            >> xml::end;
    }
    catch( ... )
    {
        // NOTHING (file not present)
    }
    QShortcut* s = new QShortcut( QKeySequence( Qt::Key_F1 ), root );
    s->setContext( Qt::ApplicationShortcut );
    connect( s, SIGNAL( activated() ), this, SLOT( ShowHelp() ) );
}

// -----------------------------------------------------------------------------
// Name: HelpSystem destructor
// Created: AGE 2008-08-18
// -----------------------------------------------------------------------------
HelpSystem::~HelpSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HelpSystem::ReadWidget
// Created: AGE 2008-08-18
// -----------------------------------------------------------------------------
void HelpSystem::ReadWidget( xml::xistream& xis )
{
    anchors_[ xis.attribute< std::string >( "name" ) ] = xis.attribute< std::string >( "page" );
}

// -----------------------------------------------------------------------------
// Name: HelpSystem::FindWidget
// Created: AGE 2008-08-18
// -----------------------------------------------------------------------------
std::string HelpSystem::FindWidget( const QObject* root )
{
    if( root )
    {
        const QWidget* widget = dynamic_cast< const QWidget* >( root );
        if( widget )
        {
            CIT_Anchors it = anchors_.find( widget->name() );
            if( it != anchors_.end() )
                return it->second;
        }
        return FindWidget( root->parent() );//root
    }
    CIT_Anchors it = anchors_.find( "default" );
    if( it != anchors_.end() )
        return it->second;
    return std::string();
}

namespace
{
    struct HelpFile
    {
        tools::Path name_;
        tools::Path default_;
    };
}

// -----------------------------------------------------------------------------
// Name: HelpSystem::RetrieveHelpFile
// Created: ABR 2014-01-13
// -----------------------------------------------------------------------------
void HelpSystem::RetrieveHelpFile()
{
    const tools::Path locale = tools::Path( tools::Language::Current().c_str() );
    const HelpFile user = { tools::Path::FromUTF8( tools::translate( "gui::HelpSystem", "Sword_General_User_Guide" ).toStdString() ),
                            "help/en/Sword_General_User_Guide.pdf" };
    const HelpFile player = { tools::Path::FromUTF8( tools::translate( "gui::HelpSystem", "Sword_Gaming_Player_Guide" ).toStdString() ),
                              "help/en/Sword_Gaming_Player_Guide.pdf" };
    const HelpFile supervisor = { tools::Path::FromUTF8( tools::translate( "gui::HelpSystem", "Sword_Gaming_Supervisor_Guide" ).toStdString() ),
                                  "help/en/Sword_Gaming_Supervisor_Guide.pdf" };
    HelpFile current = user;
    if( modeController_ && ( modeController_->GetCurrentMode() & eModes_Gaming ) != 0 )
        current = profile_ && profile_->IsSupervision() ? supervisor : player;

    helpFile_ = tools::GeneralConfig::BuildResourceChildFile( tools::Path( "help" ) / locale / current.name_ + ".pdf" );
    if( !helpFile_.Exists() )
        helpFile_ = tools::GeneralConfig::BuildResourceChildFile( current.default_ );
    if( !helpFile_.Exists() )
        helpFile_ = tools::GeneralConfig::BuildResourceChildFile( user.default_ );
}

// -----------------------------------------------------------------------------
// Name: HelpSystem::ShowHelp
// Created: AGE 2008-08-18
// -----------------------------------------------------------------------------
void HelpSystem::ShowHelp()
{
    RetrieveHelpFile();
    if( !helpFile_.Exists() )
        return;

    // pdf help
    if( !QDesktopServices::openUrl( QUrl( helpFile_.ToUTF8().c_str() ) ) )
        QMessageBox::warning( 0, tools::translate( "gui::HelpSystem", "Error" ), tools::translate( "gui::HelpSystem", "Error opening help file '%1'. Make sure you have a PDF viewer installed on your computer." ).arg( helpFile_.ToUTF8().c_str() ) );

    // chm help (not to be removed, temporarily commented)
    //const std::string page = FindWidget( qApp->widgetAt( QCursor::pos() ) );
    //if( !page.empty() )
    //    page += helpFile_ + std::string( "::/Sword_General_User_Guide/" ) + page + "/" + page + ".htm";
    //HtmlHelp( 0, page.c_str(), HH_DISPLAY_TOPIC, NULL);
}
