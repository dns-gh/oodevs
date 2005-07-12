//*****************************************************************************
//
// $Created: AML 03-03-26 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_NetMgr.cpp $
// $Author: Nld $
// $Modtime: 12/11/03 10:00 $
// $Revision: 13 $
// $Workfile: LAU_NetMgr.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"
#include "LAU_NetMgr.h"

#ifndef USE_INLINE
#   include "LAU_NetMgr.inl"
#endif

#include "LAU.h"
#include "LAU_ConMgr.h"
#include "LAU_MsgMgr.h"


using namespace NEK;
using namespace DIN;

//-----------------------------------------------------------------------------
// Name   : LAU_NetMgr constructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
LAU_NetMgr::LAU_NetMgr( LAU_Workspace& workspace, MT_InputArchive_ABC& archive )
    : dinEngine_            (                                               )
    , workspace_            (   workspace                                   )
    , nPortMos_             (   LAU_DEFAULT_NUM_PORT_MOS                    )
    , nPortSim_             (   LAU_DEFAULT_NUM_PORT_SIM                    )
    , nMagic_               (   LAU_DEFAULT_MAGIC_CONNECTION                )
    , connectionProtocols_  (   NEK_Protocols::eTCP, NEK_Protocols::eIPv4   )
{
#ifdef _DEBUG
////    printf( "%d\n", dinEngine_.GetEngineConfig().GetMessageListCriticalValue() );
////    printf( "%d\n", dinEngine_.GetEngineConfig().GetSessionManagerFlushPeriod().GetNbrSeconds() );
////    printf( "%d\n", dinEngine_.GetEngineConfig().GetNbrIncomingEventsProcessedByFrame() );
#endif // _DEBUG

    MT_LOG_INFO_MSG( "Initializing LAU_NetMgr..." );

    int nValue;

    if( !archive.Section( "Net" ) )
    {
        throw MT_ScipioException( "LAU_Workspace::Constructor", __FILE__, __LINE__, "Section 'Path' doesn't exist", archive.RetrieveLastError()->GetInfo() ); 
    }

        if( archive.ReadField( "PortMos", nValue ) )
        {
            nPortMos_ = (unsigned short)nValue; // warning C4701: local variable 'nValue' may be used without having been initialized => ok
        }
        else
        {
            throw MT_ScipioException( "LAU_NetMgr::Constructor", __FILE__, __LINE__, "Field 'PortMos' doesn't exist", archive.RetrieveLastError()->GetInfo() );
        }


        if( archive.ReadField( "PortSim", nValue ) )
        {
            nPortSim_ = (unsigned short)nValue;            
        }
        else
        {
            throw MT_ScipioException( "LAU_NetMgr::Constructor", __FILE__, __LINE__, "Field 'PortSim' doesn't exist", archive.RetrieveLastError()->GetInfo() );
        }


        if( archive.ReadField( "Magic", nValue ) )
        {
            nMagic_ = nValue;            
        }
        else
        {
            throw MT_ScipioException( "LAU_NetMgr::Constructor", __FILE__, __LINE__, "Field 'Magic' doesn't exist", archive.RetrieveLastError()->GetInfo() );
        }

    archive.EndSection(); // Net


    pConMgr_        = new LAU_ConMgr( *this );
    pMsgMgr_        = new LAU_MsgMgr( *this );
    
    MT_LOG_INFO_MSG( MT_FormatString( "LAU_NetMgr started with ID %d at port: %d/%d (magic: %d)" , dinEngine_.GetStationID(), nPortMos_, nPortSim_, nMagic_ ).c_str() );
    


}



//-----------------------------------------------------------------------------
// Name   : LAU_NetMgr destructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
LAU_NetMgr::~LAU_NetMgr()
{
    delete pConMgr_;
    delete pMsgMgr_;
}


//-----------------------------------------------------------------------------
// Name   : LAU_NetMgr::Update
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-02
//-----------------------------------------------------------------------------
void LAU_NetMgr::Update()
{
//    MT_LOG_INFO_MSG( "LAU_NetMgr Updating..." );

    dinEngine_.Update();
}
