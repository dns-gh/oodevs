// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_ABC_OrganiserAccueilColonneRefugies_h_
#define __Mission_Pawn_ABC_OrganiserAccueilColonneRefugies_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_ABC_OrganiserAccueilColonneRefugies
    @brief  Mission_Pawn_ABC_OrganiserAccueilColonneRefugies
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_ABC_OrganiserAccueilColonneRefugies : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ABC_OrganiserAccueilColonneRefugies( Pawn& target );
    virtual ~Mission_Pawn_ABC_OrganiserAccueilColonneRefugies();
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
    Mission_Pawn_ABC_OrganiserAccueilColonneRefugies( const Mission_Pawn_ABC_OrganiserAccueilColonneRefugies& );            //!< Copy constructor
    Mission_Pawn_ABC_OrganiserAccueilColonneRefugies& operator=( const Mission_Pawn_ABC_OrganiserAccueilColonneRefugies& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ABC_OrganiserAccueilColonneRefugies_h_
