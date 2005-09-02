// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-11 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Mission_Automat_ABC_h_
#define __Mission_Automat_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Action.h"
#include "Messages/ASN_Messages.h"
#include "Tools/IDManager.h"

namespace TEST {

    class Automat;
    class Scheduler;

// =============================================================================
/** @class  Mission_Automat_ABC
    @brief  Mission_Automat_ABC
    @par    Using example
    @code
    Mission_Automat_ABC;
    @endcode
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
             Mission_Automat_ABC( Automat& target );
    virtual ~Mission_Automat_ABC();
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
    MOS_ASN_MsgAutomateOrder asnMsg_;
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_ABC_h_
