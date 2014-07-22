// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawingHelper.h"
#include "DrawingTypes.h"
#include "DrawingCategory.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Location_ABC.h"

#include <xeumeuleu/xml.hpp>

namespace gui
{

const gui::DrawingTemplate& ReadStyle( xml::xistream& xis, const DrawingTypes& types )
{
    std::string category, style;
    xis >> xml::attribute( "category", category )
        >> xml::attribute( "template", style );
    return types.Get( category.c_str() ).GetTemplate( style );
}

QColor ReadColor( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "color" );
    return QColor( name.c_str() );
}

E_Dash_style ConvertStyle( const std::string& name )
{
    if( name == "dashed" )
        return gui::eDashed;
    if( name == "dash_dot" )
        return gui::eDash_dot;
    return gui::eSolid;
}

E_Dash_style ReadDashStyle( xml::xistream& xis )
{
    std::string style;
    xis >> xml::optional
        >> xml::attribute( "style", style );
    return ConvertStyle( style );
}

namespace
{
    void ReadPoint( xml::xistream& xis, kernel::Location_ABC& location, const kernel::CoordinateConverter_ABC& converter )
    {
        geometry::Point2f point;
        if( xis.has_attribute( "x" ) && xis.has_attribute( "y" ) )
        {
            float x, y;
            xis >> xml::attribute( "x", x )
                >> xml::attribute( "y", y );
            point = geometry::Point2f( x, y );
        }
        else
        {
            double latitude, longitude;
            xis >> xml::attribute( "latitude", latitude )
                >> xml::attribute( "longitude", longitude );
            point = converter.ConvertFromGeo( geometry::Point2d( longitude, latitude ) );
        }
        location.AddPoint( point );
    }
}

void ReadLocation( xml::xistream& xis, kernel::Location_ABC& location, const kernel::CoordinateConverter_ABC& converter )
{
    xis >> xml::list( "point", boost::bind( &ReadPoint, _1, boost::ref( location ), boost::cref( converter ) ) );
    if( xis.has_child( "text" ) )
    {
        std::string text, value;
        QFont font;
        xis >> xml::start( "text" )
                >> xml::attribute( "font", value )
                >> text
            >> xml::end;
        font.fromString( value.c_str() );
        location.AddText( text.c_str(), font );
    }
}

}
