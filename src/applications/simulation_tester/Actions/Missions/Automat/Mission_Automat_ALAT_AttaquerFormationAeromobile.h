// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Automat_ALAT_AttaquerFormationAeromobile_h_
#define __Mission_Automat_ALAT_AttaquerFormationAeromobile_h_

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_ALAT_AttaquerFormationAeromobile
    @brief  Mission_Automat_ALAT_AttaquerFormationAeromobile
*/
// Created: SBO 2005-08-08
// Generated:
// =============================================================================
class Mission_Automat_ALAT_AttaquerFormationAeromobile : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_ALAT_AttaquerFormationAeromobile( Automat& target );
    virtual ~Mission_Automat_ALAT_AttaquerFormationAeromobile();
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
    Mission_Automat_ALAT_AttaquerFormationAeromobile( const Mission_Automat_ALAT_AttaquerFormationAeromobile& );            //!< Copy constructor
    Mission_Automat_ALAT_AttaquerFormationAeromobile& operator=( const Mission_Automat_ALAT_AttaquerFormationAeromobile& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_ALAT_AttaquerFormationAeromobile_h_
