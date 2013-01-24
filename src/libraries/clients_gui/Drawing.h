// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_Drawing_h_
#define __gui_Drawing_h_

#include "ShapeHandler_ABC.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/EntityImplementation.h"

namespace xml
{
    class xostream;
}

namespace gui
{
    class GlTools_ABC;
    class ParametersLayer;
}

namespace gui
{
// =============================================================================
/** @class  Drawing
    @brief  Drawing
*/
// Created: SBO 2008-05-29
// =============================================================================
class Drawing : public kernel::EntityImplementation< kernel::Drawing_ABC >
              , public ShapeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Drawing( kernel::Controller& controller, unsigned long id, const QString& name );
    virtual ~Drawing();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;

    virtual void ChangeColor( const QColor& color ) = 0;
    virtual void Edit( ParametersLayer& parameters ) = 0;
    virtual void Draw( const geometry::Rectangle2f& viewport, const GlTools_ABC& tools, bool overlined ) const = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void Create() = 0;
    virtual void Update() = 0;
    //@}
};

}

#endif // __gui_Drawing_ABC_h_
