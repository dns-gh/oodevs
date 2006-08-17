// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_JOINT_MARINE_Transporter_h_
#define __Mission_Pawn_JOINT_MARINE_Transporter_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_JOINT_MARINE_Transporter
    @brief  Mission_Pawn_JOINT_MARINE_Transporter
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_JOINT_MARINE_Transporter : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_JOINT_MARINE_Transporter( Pawn& target );
    virtual ~Mission_Pawn_JOINT_MARINE_Transporter();
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
    Mission_Pawn_JOINT_MARINE_Transporter( const Mission_Pawn_JOINT_MARINE_Transporter& );            //!< Copy constructor
    Mission_Pawn_JOINT_MARINE_Transporter& operator=( const Mission_Pawn_JOINT_MARINE_Transporter& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_JOINT_MARINE_Transporter_h_
