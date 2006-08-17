// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_ASY_SInfiltrer_h_
#define __Mission_Pawn_ASY_SInfiltrer_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_ASY_SInfiltrer
    @brief  Mission_Pawn_ASY_SInfiltrer
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_ASY_SInfiltrer : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ASY_SInfiltrer( Pawn& target );
    virtual ~Mission_Pawn_ASY_SInfiltrer();
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
    Mission_Pawn_ASY_SInfiltrer( const Mission_Pawn_ASY_SInfiltrer& );            //!< Copy constructor
    Mission_Pawn_ASY_SInfiltrer& operator=( const Mission_Pawn_ASY_SInfiltrer& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ASY_SInfiltrer_h_
