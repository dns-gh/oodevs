// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Mission_Population_ABC_h_
#define __Mission_Population_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Action.h"
#include "Messages/ASN_Messages.h"
#include "Tools/IDManager.h"

namespace TEST {

    class Population;
    class Scheduler;

// =============================================================================
// Created: SBO 2005-08-08
// =============================================================================
class Mission_Population_ABC : public Action< Population >
{

public:
    //! @name ID Managers
    //@{
    static IDManager idManager_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Population_ABC( Population& target );
    virtual ~Mission_Population_ABC();
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
    MOS_ASN_MsgPopulationOrder asnMsg_;
    //@}
};

} // end namespace TEST

#endif // __Mission_Population_ABC_h_
