//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_MOSIDManager.h $
// $Author: Nld $
// $Modtime: 5/04/05 11:39 $
// $Revision: 3 $
// $Workfile: MIL_MOSIDManager.h $
//
//*****************************************************************************

#ifndef __MIL_MOSIDManager_h_
#define __MIL_MOSIDManager_h_

#include "MIL.h"

#include "MT/MT_Tools/MT_IdentifierManager.h"

//=============================================================================
// Created: NLD 2003-02-28
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_MOSIDManager
{
    MT_COPYNOTALLOWED( MIL_MOSIDManager )
   
public:
     MIL_MOSIDManager();
    ~MIL_MOSIDManager();

    //-------------------------------------------------------------------------
    /** @name ID management */
    //-------------------------------------------------------------------------
    //@{
    bool IsMosIDValid( uint nID );
    bool IsSimIDValid( uint nID );

    bool LockMosID   ( uint nID );
    bool ReleaseMosID( uint nID );

    uint GetFreeSimID();
    bool LockSimID   ( uint nID );
    void ReleaseSimID( uint nID );
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void SetClassID         ( uint nMOSClassID );
    uint GetClassID         () const;
    uint ConvertSimIDToMosID( uint nID );
    //@}


private:
    MT_IdentifierManager mosIDManager_; 
//    MT_IdentifierManager simIDManager_; 

    uint   nNextSimInstanceID_;
    uint   nMOSClassID_;

private:
    static uint nSimStartID_;
};

#   include "MIL_MOSIDManager.inl"
#endif // __MIL_MOSIDManager_h_
