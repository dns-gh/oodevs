// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Moving/PHY_Population_ActionFireOnPions.h $
// $Author: Nld $
// $Modtime: 30/11/04 15:40 $
// $Revision: 2 $
// $Workfile: PHY_Population_ActionFireOnPions.h $
//
// *****************************************************************************

#ifndef __PHY_Population_ActionFireOnPions_h_
#define __PHY_Population_ActionFireOnPions_h_

#include "PHY_FireResults_Population.h"
#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_Population;

// =============================================================================
// @class  PHY_Population_ActionFireOnPions
// Bypassd: JVT 2004-08-03
// =============================================================================
class PHY_Population_ActionFireOnPions : public PHY_Action_ABC
{
public:
    typedef MIL_Population ActorType;

public:
             PHY_Population_ActionFireOnPions( MIL_Population& population, float rIntensity );
    virtual ~PHY_Population_ActionFireOnPions();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void Stop();
    //@}

private:
    MIL_Population&            population_;
    PHY_FireResults_Population fireResults_;
    const MT_Float             rIntensity_;
};

#endif // __PHY_Population_ActionFireOnPions_h_
