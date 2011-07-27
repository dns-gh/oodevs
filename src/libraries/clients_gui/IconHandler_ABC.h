// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IconHandler_ABC_h_
#define __IconHandler_ABC_h_

#include <boost/noncopyable.hpp>

namespace gui
{
    class SymbolIcon;

// =============================================================================
/** @class  IconHandler_ABC
    @brief  Icon handler definition
*/
// Created: AGE 2006-11-22
// =============================================================================
class IconHandler_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             IconHandler_ABC() {}
    virtual ~IconHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void AddIcon( const SymbolIcon& task, const QPixmap& icon ) = 0;
    //@}
};

}

#endif // __IconHandler_ABC_h_
