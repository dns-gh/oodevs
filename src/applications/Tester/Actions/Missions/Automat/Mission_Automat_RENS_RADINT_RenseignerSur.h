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

#ifndef __Mission_Automat_RENS_RADINT_RenseignerSur_h_
#define __Mission_Automat_RENS_RADINT_RenseignerSur_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_RENS_RADINT_RenseignerSur
    @brief  Mission_Automat_RENS_RADINT_RenseignerSur
    @par    Using example
    @code
    Mission_Automat_RENS_RADINT_RenseignerSur;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-26 - 18:35:00
// =============================================================================
class Mission_Automat_RENS_RADINT_RenseignerSur : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_RENS_RADINT_RenseignerSur( Automat& target );
    virtual ~Mission_Automat_RENS_RADINT_RenseignerSur();
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
    Mission_Automat_RENS_RADINT_RenseignerSur( const Mission_Automat_RENS_RADINT_RenseignerSur& );            //!< Copy constructor
    Mission_Automat_RENS_RADINT_RenseignerSur& operator=( const Mission_Automat_RENS_RADINT_RenseignerSur& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_RENS_RADINT_RenseignerSur_h_
