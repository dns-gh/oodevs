// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Automat_ABC_h_
#define __Mission_Automat_ABC_h_

#include "Entities/Automat.h"
#include "Actions/Action.h"
#include "Messages/ASN_Messages.h"
#include "Tools/IDManager.h"

namespace TEST {

    class Scheduler;

// =============================================================================
/** @class  Mission_Automat_ABC
    @brief  Mission Automat base class
*/
// Created: SBO 2005-08-08
// =============================================================================
class Mission_Automat_ABC : public Action< Automat >
{

public:
    //! @name ID Managers
    //@{
    static IDManager idManager_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_ABC( const std::string& name, Automat& target );
    virtual ~Mission_Automat_ABC();
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
    MOS_ASN_MsgAutomateOrder asnMsg_;
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_ABC_h_
