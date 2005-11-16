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

#ifndef __Mission_Pawn_Franchir_h_
#define __Mission_Pawn_Franchir_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_Franchir
    @brief  Mission_Pawn_Franchir
    @par    Using example
    @code
    Mission_Pawn_Franchir;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_Franchir : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_Franchir( Pawn& target );
    virtual ~Mission_Pawn_Franchir();
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
    Mission_Pawn_Franchir( const Mission_Pawn_Franchir& );            //!< Copy constructor
    Mission_Pawn_Franchir& operator=( const Mission_Pawn_Franchir& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_Franchir_h_
