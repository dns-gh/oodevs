// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Mission_Automat_ALAT_DonnerCoupArret_h_
#define __Mission_Automat_ALAT_DonnerCoupArret_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_ALAT_DonnerCoupArret
    @brief  Mission_Automat_ALAT_DonnerCoupArret
    @par    Using example
    @code
    Mission_Automat_ALAT_DonnerCoupArret;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-19 - 11:31:01
// =============================================================================
class Mission_Automat_ALAT_DonnerCoupArret : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_ALAT_DonnerCoupArret( Automat& target );
    virtual ~Mission_Automat_ALAT_DonnerCoupArret();
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
    Mission_Automat_ALAT_DonnerCoupArret( const Mission_Automat_ALAT_DonnerCoupArret& );            //!< Copy constructor
    Mission_Automat_ALAT_DonnerCoupArret& operator=( const Mission_Automat_ALAT_DonnerCoupArret& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_ALAT_DonnerCoupArret_h_
