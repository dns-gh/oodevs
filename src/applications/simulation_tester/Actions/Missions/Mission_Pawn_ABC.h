// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_ABC_h_
#define __Mission_Pawn_ABC_h_

#include "Entities/Pawn.h"
#include "Actions/Action.h"
#include "Messages/ASN_Messages.h"
#include "Tools/IDManager.h"

namespace TEST {

    class Scheduler;

// =============================================================================
/** @class  Mission_Pawn_ABC
    @brief  Mission Pawn base class
*/
// Created: SBO 2005-08-08
// =============================================================================
class Mission_Pawn_ABC : public Action< Pawn >
{

public:
    //! @name ID Managers
    //@{
    static IDManager idManager_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ABC( const std::string& name, Pawn& target );
    virtual ~Mission_Pawn_ABC();
    //@}

protected:
    //! @name Action validation and commit
    //@{
    virtual void Serialize();
    virtual void Clean    ();
    void Send( const Scheduler& scheduler );
    //@}

protected:
    //! @name Member data
    //@{
    MOS_ASN_MsgPionOrder asnMsg_;
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ABC_h_
