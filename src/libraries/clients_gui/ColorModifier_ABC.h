// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColorModifier_ABC_h_
#define __ColorModifier_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
// =============================================================================
/** @class  ColorModifier_ABC
    @brief  Color modifier definition
*/
// Created: AGE 2008-05-14
// =============================================================================
class ColorModifier_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ColorModifier_ABC() {}
    virtual ~ColorModifier_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QColor Apply( const kernel::Entity_ABC& entity, const QColor& base ) const = 0;
    //@}
};

}

#endif // __ColorModifier_ABC_h_
