// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_Action_ABC_h_
#define __PHY_Action_ABC_h_

#include "MIL.h"
#include "Tools/MIL_IDManager.h"

namespace xml
{
    class xostream;
}

class PHY_Actor;

// =============================================================================
// @class  PHY_Action_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Action_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_Action_ABC();
    virtual ~PHY_Action_ABC();
    //@}

    //! @name Types
    //@{
    enum E_State
    {
        eError = 0,
        eDone = 1,
        eRunning = 2
    };
    //@}

    //! @name Operations
    //@{
    unsigned int GetId() const;
    virtual void Suspend();
    virtual void Resume();
    virtual E_State GetState() const;
    void Update();
    virtual void Stop();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Execute() = 0;
    virtual void ExecuteSuspended() = 0;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    bool bSuspended_;
    static MIL_IDManager idManager_;
    //@}
};

namespace directia
{
    void UsedByDIA( PHY_Action_ABC* );
    void ReleasedByDIA( PHY_Action_ABC* );
}

#endif // __PHY_Action_ABC_h_
