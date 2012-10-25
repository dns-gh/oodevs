// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "IconsGaugeType.h"
#include "GaugeNormalizer.h"
#include "clients_kernel/Displayer_ABC.h"
#include "tools/GeneralConfig.h"
#include <boost/filesystem/operations.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: IconsGaugeType constructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
IconsGaugeType::IconsGaugeType( xml::xistream& xis )
    : GaugeType( xis )
{
    LoadIcons( xis );
}

// -----------------------------------------------------------------------------
// Name: IconsGaugeType destructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
IconsGaugeType::~IconsGaugeType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IconsGaugeType::LoadIcons
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
void IconsGaugeType::LoadIcons( xml::xistream& xis )
{
    const std::string path = xis.attribute< std::string >( "path" );
    xis >> xml::list( "icon", *this, &IconsGaugeType::LoadIcon, path );
}

// -----------------------------------------------------------------------------
// Name: IconsGaugeType::LoadIcon
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
void IconsGaugeType::LoadIcon( xml::xistream& xis, const std::string& path )
{
    const std::string filename( tools::GeneralConfig::BuildResourceChildFile( path + "/" + xis.attribute< std::string >( "file" ) ) );
    icons_[ xis.attribute< double >( "key" ) ] = QPixmap( filename.c_str() );
    if( icons_[ xis.attribute< double >( "key" ) ].isNull() )
        throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: IconsGaugeType::Icon
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
QPixmap IconsGaugeType::Icon( double key ) const
{
    T_Icons::const_iterator it = icons_.find( key );
    if( it != icons_.end() )
        return it->second;
    return QPixmap();
}

// -----------------------------------------------------------------------------
// Name: IconsGaugeType::Display
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void IconsGaugeType::Display( kernel::Displayer_ABC& displayer, double key ) const
{
    displayer.Display( Icon( key ) );
}

// -----------------------------------------------------------------------------
// Name: IconsGaugeType::Display
// Created: JSR 2012-10-25
// -----------------------------------------------------------------------------
void IconsGaugeType::Display( QTreeWidgetItem* item, kernel::DisplayExtractor_ABC& /*extractor*/, int col, double value ) const
{
    item->setData( col, Qt::DecorationRole, Icon( value ) );
}
