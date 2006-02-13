// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-16 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ActionContext.h $
// $Author: Ape $
// $Modtime: 21/06/04 16:44 $
// $Revision: 3 $
// $Workfile: ActionContext.h $
//
// *****************************************************************************

#ifndef __ActionContext_h_
#define __ActionContext_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "SelectedElement.h"


// =============================================================================
/** @class  ActionContext
    @brief  Small class representing a GUI action context: a selected element and
            a map point.

    //$$$$ Small class, could be removed...
*/
// Created: APE 2004-04-16
// =============================================================================
class ActionContext
{
    MT_COPYNOTALLOWED( ActionContext );

public:
    //! @name Constructors/Destructor
    //@{
     ActionContext( MT_Vector2D* pPoint );
     ActionContext( SelectedElement& selectedElement, MT_Vector2D* pPoint = 0 );
    ~ActionContext();
    //@}

public:
    //! @name Public data for easy access
    //@{
    const SelectedElement selectedElement_; 
    MT_Vector2D const* pPoint_;
    //@}
};

#endif // __ActionContext_h_
