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

#ifndef __Mission_Automat_SecuriserZoneContrePopulations_h_
#define __Mission_Automat_SecuriserZoneContrePopulations_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_SecuriserZoneContrePopulations
    @brief  Mission_Automat_SecuriserZoneContrePopulations
    @par    Using example
    @code
    Mission_Automat_SecuriserZoneContrePopulations;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Automat_SecuriserZoneContrePopulations : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_SecuriserZoneContrePopulations( Automat& target );
    virtual ~Mission_Automat_SecuriserZoneContrePopulations();
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
    Mission_Automat_SecuriserZoneContrePopulations( const Mission_Automat_SecuriserZoneContrePopulations& );            //!< Copy constructor
    Mission_Automat_SecuriserZoneContrePopulations& operator=( const Mission_Automat_SecuriserZoneContrePopulations& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_SecuriserZoneContrePopulations_h_
