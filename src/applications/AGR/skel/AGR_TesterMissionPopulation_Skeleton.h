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

#ifndef __Mission_Population_$MissionName$_h_
#define __Mission_Population_$MissionName$_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Population_ABC.h"

namespace TEST {

    class Population;

// =============================================================================
/** @class  Mission_Population_$MissionName$
    @brief  Mission_Population_$MissionName$
    @par    Using example
    @code
    Mission_Population_$MissionName$;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Population_$MissionName$ : public Mission_Population_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Population_$MissionName$( Population& target );
    virtual ~Mission_Population_$MissionName$();
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
    Mission_Population_$MissionName$( const Mission_Population_$MissionName$& );            //!< Copy constructor
    Mission_Population_$MissionName$& operator=( const Mission_Population_$MissionName$& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Population_$MissionName$_h_
