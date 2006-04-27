// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_ALAT_Helitransporter_h_
#define __Mission_Pawn_ALAT_Helitransporter_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_ALAT_Helitransporter
    @brief  Mission_Pawn_ALAT_Helitransporter
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_ALAT_Helitransporter : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ALAT_Helitransporter( Pawn& target );
    virtual ~Mission_Pawn_ALAT_Helitransporter();
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
    Mission_Pawn_ALAT_Helitransporter( const Mission_Pawn_ALAT_Helitransporter& );            //!< Copy constructor
    Mission_Pawn_ALAT_Helitransporter& operator=( const Mission_Pawn_ALAT_Helitransporter& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ALAT_Helitransporter_h_
