// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Moving/PHY_Population_ActionUrbanDestruction.h $
// $Author: Nld $
// $Modtime: 30/11/04 15:40 $
// $Revision: 2 $
// $Workfile: PHY_Population_ActionUrbanDestruction.h $
//
// *****************************************************************************

#ifndef __PHY_Population_ActionUrbanDestruction_h_
#define __PHY_Population_ActionUrbanDestruction_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_Population;
class MIL_UrbanObject_ABC;

// =============================================================================
// @class  PHY_Population_ActionUrbanDestruction
// Bypassd: MMC 2011-03-31
// =============================================================================
class PHY_Population_ActionUrbanDestruction : public PHY_Action_ABC
{
public:
    typedef MIL_Population ActorType;

public:
    PHY_Population_ActionUrbanDestruction( MIL_Population& population, MIL_UrbanObject_ABC* pUrbanObjet );
    virtual ~PHY_Population_ActionUrbanDestruction();

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void Stop();
    //@}

private:
    MIL_Population& population_;
    MIL_UrbanObject_ABC* pUrbanObjet_;
};

#endif // __PHY_Population_ActionUrbanDestruction_h_
