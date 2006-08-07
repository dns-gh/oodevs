//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/IDManager.h $
// $Author: Ape $
// $Modtime: 3/02/04 11:09 $
// $Revision: 1 $
// $Workfile: IDManager.h $
//
//*****************************************************************************

#ifndef __IDManager_h_
#define __IDManager_h_

#include "Types.h"

#include "MT/MT_Tools/MT_IdentifierManager.h"

//=============================================================================
// Created: NLD 2003-02-28
//=============================================================================
class IDManager
{
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

#endif // __IDManager_h_
