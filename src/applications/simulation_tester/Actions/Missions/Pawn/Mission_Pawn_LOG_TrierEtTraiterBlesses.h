// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_LOG_TrierEtTraiterBlesses_h_
#define __Mission_Pawn_LOG_TrierEtTraiterBlesses_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_LOG_TrierEtTraiterBlesses
    @brief  Mission_Pawn_LOG_TrierEtTraiterBlesses
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_LOG_TrierEtTraiterBlesses : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_LOG_TrierEtTraiterBlesses( Pawn& target );
    virtual ~Mission_Pawn_LOG_TrierEtTraiterBlesses();
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
    Mission_Pawn_LOG_TrierEtTraiterBlesses( const Mission_Pawn_LOG_TrierEtTraiterBlesses& );            //!< Copy constructor
    Mission_Pawn_LOG_TrierEtTraiterBlesses& operator=( const Mission_Pawn_LOG_TrierEtTraiterBlesses& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_LOG_TrierEtTraiterBlesses_h_
