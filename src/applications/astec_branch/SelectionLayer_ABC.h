// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SelectionLayer_ABC_h_
#define __SelectionLayer_ABC_h_

#include "graphics/MapLayer_ABC.h"

// =============================================================================
/** @class  SelectionLayer_ABC
    @brief  Base class for layers with selectionable elements
*/
// Created: SBO 2006-03-16
// =============================================================================
class SelectionLayer_ABC : public MapLayer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SelectionLayer_ABC()
                 : hasFocus_( false )
             {}
    virtual ~SelectionLayer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SetFocus( bool focus ) { hasFocus_ = focus; };
    virtual bool HasFocus() const { return hasFocus_; }
    //@}

private:
    //! @name Member data
    //@{
    bool hasFocus_;
    //@}
};

#endif // __SelectionLayer_ABC_h_
