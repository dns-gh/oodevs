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

#include "MT/MT_Tools/MT_IdentifierManager.h" // $$$$ AGE 2006-08-07: 

//=============================================================================
// Created: NLD 2003-02-28
//=============================================================================
class IDManager
{
public:
    explicit IDManager( unsigned nMOSClassID, unsigned nStartInstanceID = 0 );
    virtual ~IDManager();

    //-------------------------------------------------------------------------
    /** @name ID management */
    //-------------------------------------------------------------------------
    //@{
    bool LockIdentifier       ( unsigned nIdentifier ); //!< Try to lock the given identifier
    unsigned GetFreeIdentifier();
    void ReleaseIdentifier    ( unsigned nIdentifier );
    //@}

private:
    MT_IdentifierManager idManager_; 
    unsigned             nMOSClassID_;
    unsigned             nStartInstanceID_;
};

#endif // __IDManager_h_
