// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingFactory_ABC_h_
#define __DrawingFactory_ABC_h_

#include "DashStyle.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Drawing_ABC;
    class Entity_ABC;
    class Location_ABC;
}

namespace xml
{
    class xistream;
}

namespace gui
{
    class DrawingTemplate;

// =============================================================================
/** @class  DrawingFactory_ABC
    @brief  DrawingFactory_ABC
*/
// Created: SBO 2008-06-04
// =============================================================================
class DrawingFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawingFactory_ABC() {}
    virtual ~DrawingFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void CreateShape( const DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity,
                              E_Dash_style dashStyle, kernel::Location_ABC& location ) const = 0;
    virtual void CreateShape( xml::xistream& xis, const kernel::Entity_ABC* diffusionEntity ) const = 0;
    //@}
};

}

#endif // __DrawingFactory_ABC_h_
