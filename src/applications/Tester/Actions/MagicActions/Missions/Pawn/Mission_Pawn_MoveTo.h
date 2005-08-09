// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Mission_Pawn_MoveTo_h_
#define __Mission_Pawn_MoveTo_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_MoveTo
    @brief  Mission_Pawn_MoveTo
    @par    Using example
    @code
    Mission_Pawn_MoveTo;
    @endcode
*/
// Created: SBO 2005-08-04
// =============================================================================
class Mission_Pawn_MoveTo : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_MoveTo( Pawn& target, uint nExecutionTick = 0 );
    virtual ~Mission_Pawn_MoveTo();
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
    Mission_Pawn_MoveTo( const Mission_Pawn_MoveTo& );            //!< Copy constructor
    Mission_Pawn_MoveTo& operator=( const Mission_Pawn_MoveTo& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_MoveTo_h_
