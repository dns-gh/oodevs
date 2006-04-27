// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_$MissionName$_h_
#define __Mission_Pawn_$MissionName$_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_$MissionName$
    @brief  Mission_Pawn_$MissionName$
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_$MissionName$ : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_$MissionName$( Pawn& target );
    virtual ~Mission_Pawn_$MissionName$();
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
    Mission_Pawn_$MissionName$( const Mission_Pawn_$MissionName$& );            //!< Copy constructor
    Mission_Pawn_$MissionName$& operator=( const Mission_Pawn_$MissionName$& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_$MissionName$_h_
