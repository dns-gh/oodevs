// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __RichWarnWidget_ABC_h_
#define __RichWarnWidget_ABC_h_

namespace gui
{

// =============================================================================
/** @class  RichWarnWidget_ABC
    @brief  RichWarnWidget_ABC
*/
// Created: ABR 2013-06-07
// =============================================================================
class RichWarnWidget_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichWarnWidget_ABC() {}
    virtual ~RichWarnWidget_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnBlink() = 0;
    virtual void OnDone() = 0;
};

} //! namespace gui

#endif // __RichWarnWidget_ABC_h_
