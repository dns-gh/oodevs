// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_ColorEditor_ABC_h
#define gui_ColorEditor_ABC_h

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
// =============================================================================
/** @class  ColorEditor_ABC
    @brief  Color editor declaration
*/
// Created: LGY 2011-06-23
// =============================================================================
class ColorEditor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ColorEditor_ABC() {}
    virtual ~ColorEditor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Add( const kernel::Entity_ABC& entity, const QColor& color, bool applyToSubordinates = true, bool force = false ) = 0;
    virtual void Remove( const kernel::Entity_ABC& entity, bool applyToSubordinates = true, bool force = false ) = 0;
    virtual void Reset( const kernel::Entity_ABC& entity, const QColor& color ) = 0;
    //@}
};

}

#endif // gui_ColorEditor_ABC_h
