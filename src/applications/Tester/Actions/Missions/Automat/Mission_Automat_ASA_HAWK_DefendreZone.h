// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Mission_Automat_ASA_HAWK_DefendreZone_h_
#define __Mission_Automat_ASA_HAWK_DefendreZone_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_ASA_HAWK_DefendreZone
    @brief  Mission_Automat_ASA_HAWK_DefendreZone
    @par    Using example
    @code
    Mission_Automat_ASA_HAWK_DefendreZone;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-22 - 15:06:58
// =============================================================================
class Mission_Automat_ASA_HAWK_DefendreZone : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_ASA_HAWK_DefendreZone( Automat& target );
    virtual ~Mission_Automat_ASA_HAWK_DefendreZone();
    //@}

protected:
    //! @name Action validation and commit
    //@{
    virtual void Serialize();
    virtual void Clean    ();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Mission_Automat_ASA_HAWK_DefendreZone( const Mission_Automat_ASA_HAWK_DefendreZone& );            //!< Copy constructor
    Mission_Automat_ASA_HAWK_DefendreZone& operator=( const Mission_Automat_ASA_HAWK_DefendreZone& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_ASA_HAWK_DefendreZone_h_
