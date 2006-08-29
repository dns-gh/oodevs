// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IDManager_h_
#define __IDManager_h_

#include "MT/MT_Tools/MT_IdentifierManager.h" // $$$$ AGE 2006-08-07: 
// $$$$ AGE 2006-08-29: se débarasser de ce bazard si il n'y a plus de recyclage des ids
// $$$$ AGE 2006-08-29: voir avec nld

namespace kernel
{

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

}

#endif // __IDManager_h_
