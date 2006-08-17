// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_ObjectTraits.h $
// $Author: Age $
// $Modtime: 31/01/05 17:39 $
// $Revision: 1 $
// $Workfile: TER_ObjectTraits.h $
//
// *****************************************************************************

#ifndef __TER_ObjectTraits_h_
#define __TER_ObjectTraits_h_

class TER_Object_ABC;

// =============================================================================
/** @class  TER_ObjectTraits
    @brief  TER_ObjectTraits
*/
// Created: AGE 2005-01-31
// =============================================================================
class TER_ObjectTraits
{
public:
    //! @name Types
    //@{
    typedef TER_Object_ABC* T_Value;
    //@}
public:
    //! @name Operations
    //@{
    int CompareOnX( MT_Float rValue, const T_Value& pObject ) const;
    int CompareOnY( MT_Float rValue, const T_Value& pObject ) const;
    //@}
};

#endif // __TER_ObjectTraits_h_
