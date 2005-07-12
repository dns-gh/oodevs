//*****************************************************************************
//
// Name     : LAU_SrvMgr_ABC.h
//
// $Created : AML 03-03-31 $
//  $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_SrvMgr_ABC.h $
//   $Author: Aml $
//  $Modtime: 22/04/03 17:25 $
// $Revision: 8 $
// $Workfile: LAU_SrvMgr_ABC.h $
//
//*****************************************************************************

#ifndef __LAU_SrvMgr_ABC_h_
#define __LAU_SrvMgr_ABC_h_

#include "LAU.h"

namespace DIN
{
    class DIN_Server;
}

class LAU_NetMgr;
class LAU_MOS;
class LAU_SIM;

//*****************************************************************************
// Object : Abstract classe representing a manager (connection or msg manager ...)
// Created: AML 03-03-31
//*****************************************************************************
class LAU_SrvMgr_ABC
{
    MT_COPYNOTALLOWED( LAU_SrvMgr_ABC );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{

    typedef std::map< DIN::DIN_StationID, LAU_MOS* > T_ID_MOS_PtrMap;
    typedef T_ID_MOS_PtrMap::iterator IT_ID_MOS_PtrMap;
    typedef T_ID_MOS_PtrMap::const_iterator CIT_ID_MOS_PtrMap;
    typedef T_ID_MOS_PtrMap::reverse_iterator RIT_ID_MOS_PtrMap;
    typedef T_ID_MOS_PtrMap::const_reverse_iterator CRIT_ID_MOS_PtrMap;

    typedef std::map< DIN::DIN_StationID, LAU_SIM* > T_ID_SIM_PtrMap;
//    typedef std::map< uint32, LAU_SIM* > T_ID_SIM_PtrMap; // en fait une map DIN_Link* => LAU_SIM*    
    typedef T_ID_SIM_PtrMap::iterator IT_ID_SIM_PtrMap;
    typedef T_ID_SIM_PtrMap::const_iterator CIT_ID_SIM_PtrMap;
    typedef T_ID_SIM_PtrMap::reverse_iterator RIT_ID_SIM_PtrMap;
    typedef T_ID_SIM_PtrMap::const_reverse_iterator CRIT_ID_SIM_PtrMap;


    explicit LAU_SrvMgr_ABC( LAU_NetMgr& netMgr );
    virtual ~LAU_SrvMgr_ABC();
    
    //-----------------------------------------------------------------------------
    /** @name Main methods */
    //-----------------------------------------------------------------------------
    //@{
    //@}
    
    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{
    const LAU_NetMgr& GetNetMgr() const;
          LAU_NetMgr& GetNetMgr();
    //@}

protected:

    LAU_NetMgr&  netMgr_;

    // Server
    DIN::DIN_Server* pServer_;      // (not owned)


};

#ifdef USE_INLINE
#   include "LAU_SrvMgr_ABC.inl"
#endif 


#endif // __LAU_SrvMgr_ABC_h_
