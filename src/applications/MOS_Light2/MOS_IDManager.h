//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_IDManager.h $
// $Author: Ape $
// $Modtime: 3/02/04 11:09 $
// $Revision: 1 $
// $Workfile: MOS_IDManager.h $
//
//*****************************************************************************

#ifndef __MOS_IDManager_h_
#define __MOS_IDManager_h_

#include "MOS_Types.h"

#include "MT/MT_Tools/MT_IdentifierManager.h"

//=============================================================================
// Created: NLD 2003-02-28
//=============================================================================
class MOS_IDManager
{
public:
     MOS_IDManager( uint nMOSClassID, uint nStartInstanceID = 0 );
    ~MOS_IDManager();

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

#ifdef MOS_USE_INLINE
#   include "MOS_IDManager.inl"
#endif

#endif // __MOS_IDManager_h_
