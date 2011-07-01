// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Drawing_ABC_h_
#define __Drawing_ABC_h_

#include "ShapeHandler_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class GlTools_ABC;
}

namespace xml
{
    class xostream;
}

namespace gui
{
    class ParametersLayer;

// =============================================================================
/** @class  Drawing_ABC
    @brief  Drawing_ABC
*/
// Created: SBO 2008-05-29
// =============================================================================
class Drawing_ABC : public kernel::Entity_ABC
                  , public ShapeHandler_ABC
                  , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Drawing_ABC();
    virtual ~Drawing_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual QColor GetColor() const = 0;
    virtual const kernel::Entity_ABC* GetDiffusionEntity() const = 0;
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    virtual void Draw( const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools, bool overlined ) const = 0;

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;

    virtual void NotifyDestruction() const = 0;

    virtual void ChangeColor( const QColor& color ) = 0;
    virtual void Edit( ParametersLayer& parameters ) = 0;
    virtual void Serialize( xml::xostream& xos ) const = 0;

    virtual const kernel::Location_ABC& GetLocation() const = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void Create() = 0;
    virtual void Update() = 0;
    //@}
};

}

#endif // __Drawing_ABC_h_
