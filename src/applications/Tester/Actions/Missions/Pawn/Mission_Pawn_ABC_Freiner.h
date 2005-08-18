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

#ifndef __Mission_Pawn_ABC_Freiner_h_
#define __Mission_Pawn_ABC_Freiner_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_ABC_Freiner
    @brief  Mission_Pawn_ABC_Freiner
    @par    Using example
    @code
    Mission_Pawn_ABC_Freiner;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-17 - 16:32:41
// =============================================================================
class Mission_Pawn_ABC_Freiner : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ABC_Freiner( Pawn& target );
    virtual ~Mission_Pawn_ABC_Freiner();
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
    Mission_Pawn_ABC_Freiner( const Mission_Pawn_ABC_Freiner& );            //!< Copy constructor
    Mission_Pawn_ABC_Freiner& operator=( const Mission_Pawn_ABC_Freiner& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ABC_Freiner_h_
