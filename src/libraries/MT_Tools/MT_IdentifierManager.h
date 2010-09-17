// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_IdentifierManager_h_
#define __MT_IdentifierManager_h_

#include <list>

//=============================================================================
/**
    @class  MT_IdentifierManager
    @brief  This class is used to allocate and release IDs
*/
// Created: NLD 2002-05-15
//=============================================================================
class MT_IdentifierManager
{
public:
    //-------------------------------------------------------------------------
    /** @name Constructor / Destructor */
    //-------------------------------------------------------------------------
    //@{
     MT_IdentifierManager();
    ~MT_IdentifierManager();
    //@}

    //-----------------------------------------------------------------------------
    /** @name User methods */
    //-----------------------------------------------------------------------------
    //@{
    bool LockIdentifier( unsigned long nIdentifier ); //!< Try to lock the given identifier
    unsigned long GetFreeIdentifier();
    bool ReleaseIdentifier( unsigned long nIdentifier );
    void ClearIdentifierManager();
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::list< unsigned long > T_IDList;
    typedef T_IDList::iterator  IT_IDList;
    //@}

private:
    T_IDList lockedIDList_;
    unsigned long nUpperBoundID_; //!< Contains the next max ID to allocate if the freeIDStack_ is empty
};

#endif // __MT_IdentifierManager_h_
