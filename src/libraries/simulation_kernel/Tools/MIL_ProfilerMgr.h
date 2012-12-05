//*****************************************************************************
//
// $Created: AML 03-05-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_ProfilerMgr.h $
// $Author: Jvt $
// $Modtime: 12/04/05 14:55 $
// $Revision: 4 $
// $Workfile: MIL_ProfilerMgr.h $
//
//*****************************************************************************

#ifndef __MIL_ProfilerMgr_h_
#define __MIL_ProfilerMgr_h_

#include "MT_Tools/MT_Profiler.h"
#include <fstream>

class MIL_AgentPion;
class MIL_Automate;
class MIL_Population;
class MIL_Config;

//*****************************************************************************
// Created: AML 03-05-21
//*****************************************************************************
class MIL_ProfilerMgr : private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit MIL_ProfilerMgr( const MIL_Config& config );
    virtual ~MIL_ProfilerMgr();
    //@}

    //! @name Operations
    //@{
    void NotifyTickBegin( unsigned int nTick );
    void NotifyTickEnd( unsigned int nTick );
    double GetLastTickDuration();

    void NotifyDecisionUpdated( const MIL_AgentPion& pion, double rTime );
    void NotifyDecisionUpdated( const MIL_Automate& automate, double rTime );
    void NotifyDecisionUpdated( const MIL_Population& population, double rTime );
    //@}

private:
    //! @name Member data
    //@{
    const bool bEnabled_;
    MT_Profiler tickDurationProfiler_;
    std::ofstream decisionUpdateFile_;
    //@}
};

#endif // __MIL_ProfilerMgr_h_
