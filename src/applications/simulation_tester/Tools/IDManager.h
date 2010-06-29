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
     IDManager( unsigned int nMOSClassID, unsigned int nStartInstanceID = 0 );
    virtual ~IDManager();

    //-------------------------------------------------------------------------
    /** @name ID management */
    //-------------------------------------------------------------------------
    //@{
    bool LockIdentifier    ( unsigned int nIdentifier ); //!< Try to lock the given identifier
    unsigned int GetFreeIdentifier ();
    void ReleaseIdentifier ( unsigned int nIdentifier );
    //@}

private:
    MT_IdentifierManager idManager_;
    unsigned int                 nMOSClassID_;
    unsigned int                 nStartInstanceID_;
};

} // end namespace TEST

#include "IDManager.inl"

#endif // __IDManager_h_
