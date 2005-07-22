// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Callback_ABC.h $
// $Author: Nld $
// $Modtime: 8/06/05 14:07 $
// $Revision: 1 $
// $Workfile: Callback_ABC.h $
//
// *****************************************************************************

#ifndef __TICExport_Callback_ABC_h_
#define __TICExport_Callback_ABC_h_

namespace TICExport
{

// =============================================================================
/** @class  Callback_ABC
    @brief  Callback_ABC
    @par    Using example
    @code
    Callback_ABC;
    @endcode
*/
// Created: SBO 2005-05-18
// =============================================================================
class Callback_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
             Callback_ABC();
    virtual ~Callback_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void OnReceiveEndTick() = 0;
    //@}
};

} // end namespace TICExport

#endif // __TICExport_Callback_ABC_h_
