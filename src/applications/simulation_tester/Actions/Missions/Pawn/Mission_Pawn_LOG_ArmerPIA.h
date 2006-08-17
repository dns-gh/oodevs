// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_LOG_ArmerPIA_h_
#define __Mission_Pawn_LOG_ArmerPIA_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_LOG_ArmerPIA
    @brief  Mission_Pawn_LOG_ArmerPIA
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_LOG_ArmerPIA : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_LOG_ArmerPIA( Pawn& target );
    virtual ~Mission_Pawn_LOG_ArmerPIA();
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
    Mission_Pawn_LOG_ArmerPIA( const Mission_Pawn_LOG_ArmerPIA& );            //!< Copy constructor
    Mission_Pawn_LOG_ArmerPIA& operator=( const Mission_Pawn_LOG_ArmerPIA& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_LOG_ArmerPIA_h_
