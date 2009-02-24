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
#include <qobjectlist.h>
#include <xeumeuleu/xml.h>
#include <htmlhelp.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: HelpSystem constructor
// Created: AGE 2008-08-18
// -----------------------------------------------------------------------------
HelpSystem::HelpSystem( QWidget* root, const std::string& config )
    : QObject( root )
    , root_( root )
{
    xml::xifstream xis( config );
    xis >> xml::start( "widgets" )
            >> xml::list( "widget", *this, &HelpSystem::ReadWidget )
        >> xml::end();
    QAction* helpAction = new QAction( "Help", QKeySequence( Qt::Key_F1 ), this );
    connect( helpAction, SIGNAL( activated() ), this, SLOT( ShowHelp() ) );
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
    anchors_[ xml::attribute< std::string >( xis, "name" ) ] = xml::attribute< std::string >( xis, "page" );
}

// -----------------------------------------------------------------------------
// Name: HelpSystem::FindWidget
// Created: AGE 2008-08-18
// -----------------------------------------------------------------------------
std::string HelpSystem::FindWidget( const QWidget* root )
{
    if( root ) 
    {
        CIT_Anchors it = anchors_.find( root->name() );
        if( it != anchors_.end() && root->hasMouse() )
            return it->second;
        if( const QObjectList* l = root->children() )
        {
            QObjectList children( *l );
            for( const QObject* child = children.first(); child; child = children.next() )
                if( child && child->inherits( "QWidget" ) )
                {
                    const std::string result = FindWidget( static_cast< const QWidget* >( child ) );
                    if( ! result.empty() )
                        return result;
                }
        }
    }
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: HelpSystem::ShowHelp
// Created: AGE 2008-08-18
// -----------------------------------------------------------------------------
void HelpSystem::ShowHelp()
{
    std::string resource = tools::GeneralConfig::BuildResourceChildFile( tools::translate( "HelpSystem", "help/en/User Guide.chm" ).ascii() );
    const std::string page = FindWidget( root_ );
    if( !page.empty() )
        resource += "::/" + page;

    HtmlHelp( 0, resource.c_str(), HH_DISPLAY_TOPIC, NULL);
}
