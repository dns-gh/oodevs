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
#include "Tools.h"
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.hpp>
#include <htmlhelp.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: HelpSystem constructor
// Created: AGE 2008-08-18
// -----------------------------------------------------------------------------
HelpSystem::HelpSystem( QWidget* root, const std::string& config )
    : QObject( root )
    , root_  ( root )
    , locale_( tools::readLang().c_str() )
{
    try
    {
        xml::xifstream xis( config );
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

// -----------------------------------------------------------------------------
// Name: HelpSystem::ShowHelp
// Created: AGE 2008-08-18
// -----------------------------------------------------------------------------
void HelpSystem::ShowHelp()
{
    static const std::string strHelp = "help/";
    const std::string strGuide = tools::translate( "gui::HelpSystem", "Sword_General_User_Guide" ).toStdString();
    std::string resource =  tools::GeneralConfig::BuildResourceChildFile( strHelp + locale_.toStdString() + "/" + strGuide + ".chm" );
    const std::string page = FindWidget( qApp->widgetAt( QCursor::pos() ) );
    if( !page.empty() )
        resource += std::string( "::/Sword_General_User_Guide/" ) + page + "/" + page + ".htm";

    HtmlHelp( 0, resource.c_str(), HH_DISPLAY_TOPIC, NULL);
}
