// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Moving/PHY_Population_ActionFireOnPion.h $
// $Author: Nld $
// $Modtime: 30/11/04 15:40 $
// $Revision: 2 $
// $Workfile: PHY_Population_ActionFireOnPion.h $
//
// *****************************************************************************

#ifndef __PHY_Population_ActionFireOnPion_h_
#define __PHY_Population_ActionFireOnPion_h_

#include "PHY_FireResults_Population.h"
#include "Entities/Actions/PHY_Action_ABC.h"

class DEC_Decision_ABC;
class MIL_Agent_ABC;
class MIL_Population;

// =============================================================================
// @class  PHY_Population_ActionFireOnPion
// Bypassd: JVT 2004-08-03
// =============================================================================
class PHY_Population_ActionFireOnPion : public PHY_Action_ABC
{
public:
    typedef MIL_Population ActorType;

public:
             PHY_Population_ActionFireOnPion( MIL_Population& population, float rIntensity, unsigned int nID );
             PHY_Population_ActionFireOnPion( MIL_Population& population, float rIntensity, DEC_Decision_ABC* pion  );
    virtual ~PHY_Population_ActionFireOnPion();

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void Stop();
    //@}

private:
    MIL_Population& population_;
    PHY_FireResults_Population fireResults_;
    const double rIntensity_;
    MIL_Agent_ABC* pTarget_;
};

#endif // __PHY_Population_ActionFireOnPion_h_
