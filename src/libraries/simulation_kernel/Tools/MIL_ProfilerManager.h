//*****************************************************************************
//
// $Created: AML 03-05-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_ProfilerManager.h $
// $Author: Jvt $
// $Modtime: 12/04/05 14:55 $
// $Revision: 4 $
// $Workfile: MIL_ProfilerManager.h $
//
//*****************************************************************************

#ifndef __MIL_ProfilerManager_h_
#define __MIL_ProfilerManager_h_

#include <fstream>

class MIL_AgentPion;
class MIL_Automate;
class MIL_Population;
class MIL_Config;

//*****************************************************************************
// Created: AML 03-05-21
//*****************************************************************************
class MIL_ProfilerManager : private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit MIL_ProfilerManager( const MIL_Config& config );
    virtual ~MIL_ProfilerManager();
    //@}

    //! @name Operations
    //@{
    void NotifyDecisionUpdated( const MIL_AgentPion& pion, double rTime );
    void NotifyDecisionUpdated( const MIL_Automate& automate, double rTime );
    void NotifyDecisionUpdated( const MIL_Population& population, double rTime );
    //@}

private:
    //! @name Member data
    //@{
    const bool bEnabled_;
    std::ofstream decisionUpdateFile_;
    //@}
};

#endif // __MIL_ProfilerManager_h_
