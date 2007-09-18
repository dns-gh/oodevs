// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_ActionInfluence_h_
#define __PHY_ActionInfluence_h_

#include "MIL.h"
#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_AgentPion;
class PHY_RoleAction_FolkInfluence;

namespace population
{
    class Object_ABC;
}

// =============================================================================
/** @class  PHY_ActionInfluence
    @brief  PHY_ActionInfluence
*/
// Created: JCR 2007-09-12
// =============================================================================
class PHY_ActionInfluence : public PHY_Action_ABC
{
    //! @name Copy/Assignement
    //@{
    MT_COPYNOTALLOWED( PHY_ActionInfluence )
    //@}

public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionInfluence( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionInfluence();
    //@}

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    //! @name Member data
    //@{
    const MIL_AgentPion&            pion_;
    PHY_RoleAction_FolkInfluence&   role_;
    DIA_Variable_ABC&               diaReturnCode_;
    std::string                     activity_;
    double                          influence_;
    population::Object_ABC*         pObject_;
    //@}
};

#endif // __PHY_ActionInfluence_h_
