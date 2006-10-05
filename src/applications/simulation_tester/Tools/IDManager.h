// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-05 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __IDManager_h_
#define __IDManager_h_

#include "MT/MT_Tools/MT_IdentifierManager.h"

namespace TEST {

// =============================================================================
/** @class  IDManager
    @brief  IDManager
    @par    Using example
    @code
    IDManager;
    @endcode
*/
// Created: SBO 2005-08-05
// =============================================================================
class IDManager
{
    MT_COPYNOTALLOWED( IDManager )

public:
     IDManager( uint nMOSClassID, uint nStartInstanceID = 0 );
    ~IDManager();

    //-------------------------------------------------------------------------
    /** @name ID management */
    //-------------------------------------------------------------------------
    //@{
    bool LockIdentifier    ( uint nIdentifier ); //!< Try to lock the given identifier
    uint GetFreeIdentifier ();
    void ReleaseIdentifier ( uint nIdentifier );
    //@}

private:
    MT_IdentifierManager idManager_; 
    uint                 nMOSClassID_;
    uint                 nStartInstanceID_;
};

} // end namespace TEST

#include "IDManager.inl"

#endif // __IDManager_h_
