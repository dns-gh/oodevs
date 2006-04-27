// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Automat_ABC_PrendrePreciserContact_h_
#define __Mission_Automat_ABC_PrendrePreciserContact_h_

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_ABC_PrendrePreciserContact
    @brief  Mission_Automat_ABC_PrendrePreciserContact
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Automat_ABC_PrendrePreciserContact : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_ABC_PrendrePreciserContact( Automat& target );
    virtual ~Mission_Automat_ABC_PrendrePreciserContact();
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
    Mission_Automat_ABC_PrendrePreciserContact( const Mission_Automat_ABC_PrendrePreciserContact& );            //!< Copy constructor
    Mission_Automat_ABC_PrendrePreciserContact& operator=( const Mission_Automat_ABC_PrendrePreciserContact& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_ABC_PrendrePreciserContact_h_
