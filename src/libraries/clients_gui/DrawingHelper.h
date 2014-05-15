// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui_DrawingHelper_h_
#define __gui_DrawingHelper_h_

#include "DashStyle.h"

namespace xml
{
    class xistream;
}

namespace gui
{
    class DrawingTemplate;
    class DrawingTypes;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class LocationProxy;
}

namespace gui
{
    QColor ReadColor( xml::xistream& xis );
    const DrawingTemplate& ReadStyle( xml::xistream& xis, const DrawingTypes& types );
    E_Dash_style ConvertStyle( const std::string& name );
    E_Dash_style ReadDashStyle( xml::xistream& xis );
    void ReadLocation( xml::xistream& xis, kernel::LocationProxy& proxy, const kernel::CoordinateConverter_ABC& converter );
}

#endif // __gui_DrawingHelper_h_
