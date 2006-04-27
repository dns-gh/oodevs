// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage_h_
#define __Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage
    @brief  Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage( Pawn& target );
    virtual ~Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage();
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
    Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage( const Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage& );            //!< Copy constructor
    Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage& operator=( const Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ALAT_EffectuerRechercheEtSauvetage_h_
