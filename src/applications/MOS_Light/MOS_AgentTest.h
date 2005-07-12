//*****************************************************************************
//
// $Created: FBD 03-07-31 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentTest.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_AgentTest.h $
//
//*****************************************************************************

#ifndef __MOS_AgentTest_h_
#define __MOS_AgentTest_h_

class MOS_Agent;
class MOS_Mission_ABC;
#include "MOS_Types.h"

#define TIMEMAXINTERVAL 60 * 20 // 10 minutes

//*****************************************************************************
// Created: FBD 03-07-31
//*****************************************************************************
class MOS_AgentTest
{
    MT_COPYNOTALLOWED( MOS_AgentTest )

public:
    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor */
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_AgentTest( MOS_Agent& agent);
    virtual ~MOS_AgentTest();
    //}@

    //-------------------------------------------------------------------------
    /** @name Accessor */
    //-------------------------------------------------------------------------
    //@{
    MOS_Agent& GetAgent();
    //}@
    
    //-------------------------------------------------------------------------
    /** @name Timer */
    //-------------------------------------------------------------------------
    //@{
    void Update( int nCurtime, MT_OutputArchive_ABC& archive, int nMinimumTime );
    void UpdateRec( int nCurtime, MT_OutputArchive_ABC& archive, MOS_Mission_ABC& mission );

    void SayMissionFinished( uint nCurTime );
    //}@


private:

    MOS_Agent&          agent_;
    int                nTimeNextOrder_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_AgentTest.inl"
#endif


#endif // __MOS_AgentTest_h_