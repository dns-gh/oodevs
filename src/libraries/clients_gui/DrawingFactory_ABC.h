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

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

namespace xml
{
    class xistream;
}

namespace gui
{
    class Drawing_ABC;
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
    virtual Drawing_ABC* CreateShape( const DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity ) const = 0;
    virtual Drawing_ABC* CreateShape( xml::xistream& xis, const kernel::Entity_ABC* diffusionEntity ) const = 0;
    //@}
};

}

#endif // __DrawingFactory_ABC_h_
