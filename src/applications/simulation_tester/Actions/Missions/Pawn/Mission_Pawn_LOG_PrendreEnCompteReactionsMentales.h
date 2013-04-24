// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_LOG_PrendreEnCompteReactionsMentales_h_
#define __Mission_Pawn_LOG_PrendreEnCompteReactionsMentales_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_LOG_PrendreEnCompteReactionsMentales
    @brief  Mission_Pawn_LOG_PrendreEnCompteReactionsMentales
*/
// Created: SBO 2005-08-08
// Generated:
// =============================================================================
class Mission_Pawn_LOG_PrendreEnCompteReactionsMentales : public Mission_Pawn_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_LOG_PrendreEnCompteReactionsMentales( Pawn& target );
    virtual ~Mission_Pawn_LOG_PrendreEnCompteReactionsMentales();
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
    Mission_Pawn_LOG_PrendreEnCompteReactionsMentales( const Mission_Pawn_LOG_PrendreEnCompteReactionsMentales& );            //!< Copy constructor
    Mission_Pawn_LOG_PrendreEnCompteReactionsMentales& operator=( const Mission_Pawn_LOG_PrendreEnCompteReactionsMentales& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_LOG_PrendreEnCompteReactionsMentales_h_