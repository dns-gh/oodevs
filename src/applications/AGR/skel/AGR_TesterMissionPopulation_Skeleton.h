// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Population_$MissionName$_h_
#define __Mission_Population_$MissionName$_h_

#include "Actions/Missions/Mission_Population_ABC.h"

namespace TEST {

    class Population;

// =============================================================================
/** @class  Mission_Population_$MissionName$
    @brief  Mission_Population_$MissionName$
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
