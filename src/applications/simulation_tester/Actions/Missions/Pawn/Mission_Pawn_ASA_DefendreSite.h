// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_ASA_DefendreSite_h_
#define __Mission_Pawn_ASA_DefendreSite_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_ASA_DefendreSite
    @brief  Mission_Pawn_ASA_DefendreSite
*/
// Created: SBO 2005-08-08
// Generated:
// =============================================================================
class Mission_Pawn_ASA_DefendreSite : public Mission_Pawn_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ASA_DefendreSite( Pawn& target );
    virtual ~Mission_Pawn_ASA_DefendreSite();
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
    Mission_Pawn_ASA_DefendreSite( const Mission_Pawn_ASA_DefendreSite& );            //!< Copy constructor
    Mission_Pawn_ASA_DefendreSite& operator=( const Mission_Pawn_ASA_DefendreSite& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ASA_DefendreSite_h_