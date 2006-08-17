// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_GEN_FranchirFosseAC_h_
#define __Mission_Pawn_GEN_FranchirFosseAC_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_GEN_FranchirFosseAC
    @brief  Mission_Pawn_GEN_FranchirFosseAC
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_GEN_FranchirFosseAC : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_GEN_FranchirFosseAC( Pawn& target );
    virtual ~Mission_Pawn_GEN_FranchirFosseAC();
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
    Mission_Pawn_GEN_FranchirFosseAC( const Mission_Pawn_GEN_FranchirFosseAC& );            //!< Copy constructor
    Mission_Pawn_GEN_FranchirFosseAC& operator=( const Mission_Pawn_GEN_FranchirFosseAC& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_GEN_FranchirFosseAC_h_
