// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Action_Magic_Move_h_
#define __Action_Magic_Move_h_

#include "Entities/Pawn.h"
#include "Actions/Action.h"
#include "Messages/ASN_Messages.h"

namespace TEST {

    class Scheduler;

// =============================================================================
/** @class  Action_Magic_Move
    @brief  Action Magic Move
*/
// Created: SBO 2005-08-04
// =============================================================================
class Action_Magic_Move : public Action< Pawn >
{

public:
    //! @name Constructors/Destructor
    //@{
             Action_Magic_Move( Pawn& target );
    virtual ~Action_Magic_Move();
    //@}

protected:
    //! @name Action validation and commit
    //@{
    virtual void Serialize();
    virtual void Send     ( const Scheduler& scheduler );
    virtual void Clean    ();
    //@}

protected:
    //! @name Member data
    //@{
    MOS_ASN_MsgUnitMagicAction asnMsg_;
    //@}
};

} // end namespace TEST

#endif // __Action_Magic_Move_h_
