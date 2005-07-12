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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ActionContext.h $
// $Author: Ape $
// $Modtime: 21/06/04 16:44 $
// $Revision: 3 $
// $Workfile: MOS_ActionContext.h $
//
// *****************************************************************************

#ifndef __MOS_ActionContext_h_
#define __MOS_ActionContext_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_SelectedElement.h"


// =============================================================================
/** @class  MOS_ActionContext
    @brief  Small class representing a GUI action context: a selected element and
            a map point.

    //$$$$ Small class, could be removed...
*/
// Created: APE 2004-04-16
// =============================================================================
class MOS_ActionContext
{
    MT_COPYNOTALLOWED( MOS_ActionContext );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ActionContext( MT_Vector2D* pPoint );
     MOS_ActionContext( MOS_SelectedElement& selectedElement, MT_Vector2D* pPoint = 0 );
    ~MOS_ActionContext();
    //@}

public:
    //! @name Public data for easy access
    //@{
    const MOS_SelectedElement selectedElement_; 
    MT_Vector2D const* pPoint_;
    //@}
};


#ifdef MOS_USE_INLINE
#   include "MOS_ActionContext.inl"
#endif

#endif // __MOS_ActionContext_h_
