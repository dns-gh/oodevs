// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

#ifndef __Mission_Pawn_ASY_CommettreAttentatContrePopulation_h_
#define __Mission_Pawn_ASY_CommettreAttentatContrePopulation_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_ASY_CommettreAttentatContrePopulation
    @brief  Mission_Pawn_ASY_CommettreAttentatContrePopulation
    @par    Using example
    @code
    Mission_Pawn_ASY_CommettreAttentatContrePopulation;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_ASY_CommettreAttentatContrePopulation : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ASY_CommettreAttentatContrePopulation( Pawn& target );
    virtual ~Mission_Pawn_ASY_CommettreAttentatContrePopulation();
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
    Mission_Pawn_ASY_CommettreAttentatContrePopulation( const Mission_Pawn_ASY_CommettreAttentatContrePopulation& );            //!< Copy constructor
    Mission_Pawn_ASY_CommettreAttentatContrePopulation& operator=( const Mission_Pawn_ASY_CommettreAttentatContrePopulation& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ASY_CommettreAttentatContrePopulation_h_
