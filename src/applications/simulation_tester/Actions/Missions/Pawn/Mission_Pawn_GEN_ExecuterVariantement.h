// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_GEN_ExecuterVariantement_h_
#define __Mission_Pawn_GEN_ExecuterVariantement_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_GEN_ExecuterVariantement
    @brief  Mission_Pawn_GEN_ExecuterVariantement
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_GEN_ExecuterVariantement : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_GEN_ExecuterVariantement( Pawn& target );
    virtual ~Mission_Pawn_GEN_ExecuterVariantement();
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
    Mission_Pawn_GEN_ExecuterVariantement( const Mission_Pawn_GEN_ExecuterVariantement& );            //!< Copy constructor
    Mission_Pawn_GEN_ExecuterVariantement& operator=( const Mission_Pawn_GEN_ExecuterVariantement& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_GEN_ExecuterVariantement_h_
