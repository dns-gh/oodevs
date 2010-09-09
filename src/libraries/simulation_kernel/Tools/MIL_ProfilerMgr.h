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

class MIL_AgentPion;
class MIL_Automate;
class MIL_Population;

//*****************************************************************************
// Created: AML 03-05-21
//*****************************************************************************
class MIL_ProfilerMgr : private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit MIL_ProfilerMgr( bool bEnabled );
    virtual ~MIL_ProfilerMgr();
    //@}

    //! @name Accessors
    //@{
    bool IsProfilingEnabled() const;
    //@}

    //! @name Operations
    //@{
    void NotifyTickBegin( unsigned int nTick );
    void NotifyTickEnd( unsigned int nTick );
    double GetLastTickDuration();
    double GetAverageTickDuration();

    void NotifyDecisionUpdated( const MIL_AgentPion& pion, MT_Float rTime );
    void NotifyDecisionUpdated( const MIL_Automate& automate, MT_Float rTime );
    void NotifyDecisionUpdated( const MIL_Population& population, MT_Float rTime );
    //@}

private:
    //! @name Types
    //@{
    struct sDecFunctionProfiling
    {
        unsigned int nAgentID_;
        std::string strFunction_;
        MT_Float rTime_;
    };

    typedef std::vector< sDecFunctionProfiling >           T_DecFunctionProfilingVector;
    typedef T_DecFunctionProfilingVector::const_iterator CIT_DecFunctionProfilingVector;
    //@}

private:
    //! @name Member data
    //@{
    const bool bEnabled_;
    MT_Profiler tickDurationProfiler_;
    std::ofstream decFunctionsFile_;
    std::ofstream decisionUpdateFile_;
    //@}
};

#include "MIL_ProfilerMgr.inl"

#endif // __MIL_ProfilerMgr_h_