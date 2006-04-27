// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_ALAT_Heliporter_h_
#define __Mission_Pawn_ALAT_Heliporter_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_ALAT_Heliporter
    @brief  Mission_Pawn_ALAT_Heliporter
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_ALAT_Heliporter : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ALAT_Heliporter( Pawn& target );
    virtual ~Mission_Pawn_ALAT_Heliporter();
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
    Mission_Pawn_ALAT_Heliporter( const Mission_Pawn_ALAT_Heliporter& );            //!< Copy constructor
    Mission_Pawn_ALAT_Heliporter& operator=( const Mission_Pawn_ALAT_Heliporter& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ALAT_Heliporter_h_
