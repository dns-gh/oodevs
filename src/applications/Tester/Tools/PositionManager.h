// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/PositionManager.h $
// $Author: Sbo $
// $Modtime: 18/07/05 10:14 $
// $Revision: 6 $
// $Workfile: PositionManager.h $
//
// *****************************************************************************

#ifndef __PositionManager_h_
#define __PositionManager_h_

#include "Types.h"

namespace TEST
{

// =============================================================================
/** @class  PositionManager
    @brief  PositionManager
    @par    Using example
    @code
    PositionManager;
    @endcode
*/
// Created: SBO 2005-05-18
// =============================================================================
class PositionManager
{
    MT_COPYNOTALLOWED( PositionManager );

public:
    //! @name Operations
    //@{
    static void Initialize( const std::string& strWorldConfigFile );
    static void Terminate ();
    //@}

private:
	//! @name Constructors/Destructor
    //@{
             PositionManager();
    virtual ~PositionManager();
    //@}

};


} // end namespace TIC

#endif // __PositionManager_h_