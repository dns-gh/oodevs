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

namespace gui
{

// =============================================================================
/** @class  IconHandler_ABC
    @brief  Icon handler definition
*/
// Created: AGE 2006-11-22
// =============================================================================
class IconHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IconHandler_ABC() {};
    virtual ~IconHandler_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void AddIcon( const std::string& level, const std::string& symbol, const QColor& color, const QPixmap& icon ) = 0;
    //@}
};

}

#endif // __IconHandler_ABC_h_
