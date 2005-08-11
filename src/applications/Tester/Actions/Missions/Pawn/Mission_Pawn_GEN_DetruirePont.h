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

#ifndef __Mission_Pawn_GEN_DetruirePont_h_
#define __Mission_Pawn_GEN_DetruirePont_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_GEN_DetruirePont
    @brief  Mission_Pawn_GEN_DetruirePont
    @par    Using example
    @code
    Mission_Pawn_GEN_DetruirePont;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-11 - 11:53:19
// =============================================================================
class Mission_Pawn_GEN_DetruirePont : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_GEN_DetruirePont( Pawn& target, uint nExecutionTick = 0 );
    virtual ~Mission_Pawn_GEN_DetruirePont();
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
    Mission_Pawn_GEN_DetruirePont( const Mission_Pawn_GEN_DetruirePont& );            //!< Copy constructor
    Mission_Pawn_GEN_DetruirePont& operator=( const Mission_Pawn_GEN_DetruirePont& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_GEN_DetruirePont_h_
