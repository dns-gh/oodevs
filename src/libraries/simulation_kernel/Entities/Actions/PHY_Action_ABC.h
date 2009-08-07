// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/PHY_Action_ABC.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_Action_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_Action_ABC_h_
#define __PHY_Action_ABC_h_

#include "MIL.h"

class PHY_Actor;

// =============================================================================
// @class  PHY_Action_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Action_ABC : private boost::noncopyable
{

public:
    explicit PHY_Action_ABC( PHY_Actor& actor );
    virtual ~PHY_Action_ABC();

    //! @name Operations
    //@{
    void Suspend();
    void Resume ();
    void Update ();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Execute         () = 0;
    virtual void ExecuteSuspended() = 0;
    //@}

protected:
    PHY_Actor& actor_;

private:
    bool bSuspended_;
};

namespace directia
{
    void UsedByDIA( PHY_Action_ABC* );
    void ReleasedByDIA( PHY_Action_ABC* );
}

#endif // __PHY_Action_ABC_h_
