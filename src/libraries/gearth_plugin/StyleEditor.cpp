// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "StyleEditor.h"
#include "dispatcher/PluginConfig.h"
#include "dispatcher/Config.h"
#include "xeumeuleu/xml.h"

using namespace gearth;

// -----------------------------------------------------------------------------
// Name: StyleEditor constructor
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
StyleEditor::StyleEditor( const dispatcher::Config& config )
{
    const dispatcher::PluginConfig& plugin = config.GetPluginConfig( "gearth" );
    xml::xifstream xis( config.BuildGameChildFile( plugin.GetParameter( "styles" ) ) );
    xis >> xml::start( "kml-styles" )
            >> xml::start( "icons" )
                >> xml::list( "icon", *this, ReadStyle )
            >> xml::end()
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: StyleEditor destructor
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
StyleEditor::~StyleEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StyleEditor::ReadStyle
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
void StyleEditor::ReadStyle( xml::xistream& xis )
{
    std::string name, value;
    xis >> xml::attribute( "unit", name ) 
        >> xml::attribute( "file", value );
    styles_[ name ] = value;
}

// -----------------------------------------------------------------------------
// Name: StyleEditor::Load
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
void StyleEditor::Load( xml::xostream& xos )
{   
    for ( CIT_StyleMap it = styles_.begin(); it != styles_.end(); ++it )
        WriteEntityStyle( xos, it->first, it->second );
    
//    WriteObjectStyle( xos, "Fire", "3f0000ff" );
//    WriteLineStyle( xos, "Path", "3fff0000" );
//    WriteLineStyle( xos, "Limit", "3fff0000" );
//    WriteLineStyle( xos, "Lima", "3fff0000" );
}

// -----------------------------------------------------------------------------
// Name: FacadeKML::WriteStyle
// Created: JCR 2007-03-07
// -----------------------------------------------------------------------------
void StyleEditor::WriteEntityStyle( xml::xostream& xos, const std::string& style, const std::string& ref )
{    
    xos << xml::start( "Style" ) << xml::attribute( "id", style + "_PlaceMark" )
            << xml::start( "IconStyle" )
                << xml::start( "Icon" )
                    << xml::content( "href", ref )
                << xml::end()
            << xml::end()
         << xml::end();
}

// -----------------------------------------------------------------------------
// Name: FacadeKML::WriteObjectStyle
// Created: JCR 2007-03-12
// -----------------------------------------------------------------------------
void StyleEditor::WriteObjectStyle( xml::xostream& xos, const std::string& style, const std::string& color )
{
    xos << xml::start( "Style" ) << xml::attribute( "id", style )
            << xml::start( "PolyStyle" )                
                << xml::content( "color", color )
                << xml::content( "colorMode", "normal" )
            << xml::end()
         << xml::end();
}

// -----------------------------------------------------------------------------
// Name: FacadeKML::WriteObjectStyle
// Created: JCR 2007-03-12
// -----------------------------------------------------------------------------
void StyleEditor::WriteLineStyle( xml::xostream& xos, const std::string& style, const std::string& color )
{
    xos << xml::start( "Style" ) << xml::attribute( "id", style )
            << xml::start( "LineStyle" )                
                << xml::content( "color", color )
                << xml::content( "colorMode", "normal" )
                << xml::content( "width", 4 )
            << xml::end()
         << xml::end();
}


// -----------------------------------------------------------------------------
// Name: StyleEditor::GetStyle
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
//const std::string& StyleEditor::GetStyle( const std::string& type ) const
//{
//    // return styles_[ type ];
//}
