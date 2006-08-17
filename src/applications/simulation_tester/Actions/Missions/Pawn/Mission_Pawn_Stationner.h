// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_Stationner_h_
#define __Mission_Pawn_Stationner_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_Stationner
    @brief  Mission_Pawn_Stationner
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_Stationner : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_Stationner( Pawn& target );
    virtual ~Mission_Pawn_Stationner();
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
    Mission_Pawn_Stationner( const Mission_Pawn_Stationner& );            //!< Copy constructor
    Mission_Pawn_Stationner& operator=( const Mission_Pawn_Stationner& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_Stationner_h_
