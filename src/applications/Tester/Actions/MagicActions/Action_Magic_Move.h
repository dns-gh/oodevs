// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Action_Magic_Move_h_
#define __Action_Magic_Move_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Action.h"
#include "Messages/ASN_Messages.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Action_Magic_Move
    @brief  Action_Magic_Move
    @par    Using example
    @code
    Action_Magic_Move;
    @endcode
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
    virtual void Send     ();
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
