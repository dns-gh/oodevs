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
#include "dispatcher/Config.h"
#include <xeumeuleu/xml.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

using namespace plugins::gearth;

namespace fs = boost::filesystem;
namespace po = boost::program_options;


namespace
{
    std::string GetPath( const std::string& file )
    {
        fs::path full_path( fs::initial_path< fs::path >() );
        return full_path.native_file_string() + "/" + file;
    }
}

// -----------------------------------------------------------------------------
// Name: StyleEditor constructor
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
StyleEditor::StyleEditor( const dispatcher::Config& config, const std::string& opt_path )
{
    xml::xifstream xis( config.BuildSessionChildFile( "kml-styles.xml" ) );    
    xis >> xml::start( "kml-styles" )
        >> xml::start( "icons" ) >> xml::attribute( "small", path_small_ ) >> xml::attribute( "large", path_large_ )
                >> xml::list( "icon", *this, &StyleEditor::ReadStyle )
            >> xml::end()
        >> xml::end();

    if( opt_path == "" )
    {
        path_small_ = GetPath( config.BuildSessionChildFile( path_small_ ) );
        path_large_ = GetPath( config.BuildSessionChildFile( path_large_ ) );
    }
    else
    {
        path_small_ = opt_path + "/" + path_small_;
        path_large_ = opt_path + "/" + path_large_;
    }
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
    for( CIT_StyleMap it = styles_.begin(); it != styles_.end(); ++it )
        WriteEntityStyle( xos, it->first, it->second );

//    WriteObjectStyle( xos, "Fire", "3f0000ff" );
    WriteLineStyle( xos, "Path", "3fff0000" );
    WriteLineStyle( xos, "Limit", "3fff0000" );
    WriteLineStyle( xos, "Lima", "3fff0000" );
}

// -----------------------------------------------------------------------------
// Name: FacadeKML::WriteStyle
// Created: JCR 2007-03-07
// -----------------------------------------------------------------------------
void StyleEditor::WriteEntityStyle( xml::xostream& xos, const std::string& style, const std::string& ref )
{
    xos << xml::start( "Style" ) << xml::attribute( "id", style + "_Placemark" )
            << xml::start( "IconStyle" )
                << xml::start( "Icon" )
                    << xml::content( "href", path_small_ + ref )
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
std::string StyleEditor::GetStyle( const std::string& type ) const
{
    if( type.substr( 0, 2 ) == "HS" )
    {
        std::string ref( type, 5, type.size() );
        if( styles_.find( ref ) != styles_.end() )
            return "#" + ref + "_Placemark";
    }
    else if( type == "Lima" || type == "Limit" )
        return "#" + type;
    return "";
}
