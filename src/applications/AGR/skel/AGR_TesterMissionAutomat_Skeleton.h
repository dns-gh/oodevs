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

#ifndef __Mission_Automat_$MissionName$_h_
#define __Mission_Automat_$MissionName$_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_$MissionName$
    @brief  Mission_Automat_$MissionName$
    @par    Using example
    @code
    Mission_Automat_$MissionName$;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: $TIME$
// =============================================================================
class Mission_Automat_$MissionName$ : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_$MissionName$( Automat& target );
    virtual ~Mission_Automat_$MissionName$();
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
    Mission_Automat_$MissionName$( const Mission_Automat_$MissionName$& );            //!< Copy constructor
    Mission_Automat_$MissionName$& operator=( const Mission_Automat_$MissionName$& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_$MissionName$_h_
