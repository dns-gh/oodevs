// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_ColorController_ABC_h
#define gui_ColorController_ABC_h

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;

// =============================================================================
/** @class  ColorController_ABC
    @brief  Color controller declaration
*/
// Created: LGY 2011-06-23
// =============================================================================
class ColorController_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ColorController_ABC() {}
    virtual ~ColorController_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ApplyDefaultColor( const kernel::Entity_ABC& entity, ColorStrategy_ABC& strategy, bool applyToSubordinates ) = 0;
    virtual void Add( const kernel::Entity_ABC& entity, const QColor& color, bool applyToSubordinates = true, bool force = false ) = 0;
    virtual void Remove( const kernel::Entity_ABC& entity ) = 0;
    virtual void Reset( const kernel::Entity_ABC& entity, const QColor& color ) = 0;
    //@}
};

}

#endif // gui_ColorController_ABC_h
