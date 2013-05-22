// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Automat_ALAT_EffectuerRechercheEtSauvetage_h_
#define __Mission_Automat_ALAT_EffectuerRechercheEtSauvetage_h_

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_ALAT_EffectuerRechercheEtSauvetage
    @brief  Mission_Automat_ALAT_EffectuerRechercheEtSauvetage
*/
// Created: SBO 2005-08-08
// Generated:
// =============================================================================
class Mission_Automat_ALAT_EffectuerRechercheEtSauvetage : public Mission_Automat_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_ALAT_EffectuerRechercheEtSauvetage( Automat& target );
    virtual ~Mission_Automat_ALAT_EffectuerRechercheEtSauvetage();
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
    Mission_Automat_ALAT_EffectuerRechercheEtSauvetage( const Mission_Automat_ALAT_EffectuerRechercheEtSauvetage& );            //!< Copy constructor
    Mission_Automat_ALAT_EffectuerRechercheEtSauvetage& operator=( const Mission_Automat_ALAT_EffectuerRechercheEtSauvetage& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_ALAT_EffectuerRechercheEtSauvetage_h_