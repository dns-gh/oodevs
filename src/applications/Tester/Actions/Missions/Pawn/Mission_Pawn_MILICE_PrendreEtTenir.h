// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

#ifndef __Mission_Pawn_MILICE_PrendreEtTenir_h_
#define __Mission_Pawn_MILICE_PrendreEtTenir_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_MILICE_PrendreEtTenir
    @brief  Mission_Pawn_MILICE_PrendreEtTenir
    @par    Using example
    @code
    Mission_Pawn_MILICE_PrendreEtTenir;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-11 - 11:53:20
// =============================================================================
class Mission_Pawn_MILICE_PrendreEtTenir : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_MILICE_PrendreEtTenir( Pawn& target, uint nExecutionTick = 0 );
    virtual ~Mission_Pawn_MILICE_PrendreEtTenir();
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
    Mission_Pawn_MILICE_PrendreEtTenir( const Mission_Pawn_MILICE_PrendreEtTenir& );            //!< Copy constructor
    Mission_Pawn_MILICE_PrendreEtTenir& operator=( const Mission_Pawn_MILICE_PrendreEtTenir& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_MILICE_PrendreEtTenir_h_
