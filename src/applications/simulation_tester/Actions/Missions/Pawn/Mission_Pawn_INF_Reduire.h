// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_INF_Reduire_h_
#define __Mission_Pawn_INF_Reduire_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_INF_Reduire
    @brief  Mission_Pawn_INF_Reduire
*/
// Created: SBO 2005-08-08
// Generated:
// =============================================================================
class Mission_Pawn_INF_Reduire : public Mission_Pawn_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_INF_Reduire( Pawn& target );
    virtual ~Mission_Pawn_INF_Reduire();
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
    Mission_Pawn_INF_Reduire( const Mission_Pawn_INF_Reduire& );            //!< Copy constructor
    Mission_Pawn_INF_Reduire& operator=( const Mission_Pawn_INF_Reduire& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_INF_Reduire_h_