// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Automat_ALAT_Surveiller_h_
#define __Mission_Automat_ALAT_Surveiller_h_

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_ALAT_Surveiller
    @brief  Mission_Automat_ALAT_Surveiller
*/
// Created: SBO 2005-08-08
// Generated:
// =============================================================================
class Mission_Automat_ALAT_Surveiller : public Mission_Automat_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_ALAT_Surveiller( Automat& target );
    virtual ~Mission_Automat_ALAT_Surveiller();
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
    Mission_Automat_ALAT_Surveiller( const Mission_Automat_ALAT_Surveiller& );            //!< Copy constructor
    Mission_Automat_ALAT_Surveiller& operator=( const Mission_Automat_ALAT_Surveiller& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_ALAT_Surveiller_h_