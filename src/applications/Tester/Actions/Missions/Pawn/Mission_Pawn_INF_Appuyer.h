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

#ifndef __Mission_Pawn_INF_Appuyer_h_
#define __Mission_Pawn_INF_Appuyer_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_INF_Appuyer
    @brief  Mission_Pawn_INF_Appuyer
    @par    Using example
    @code
    Mission_Pawn_INF_Appuyer;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-12 - 15:12:57
// =============================================================================
class Mission_Pawn_INF_Appuyer : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_INF_Appuyer( Pawn& target );
    virtual ~Mission_Pawn_INF_Appuyer();
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
    Mission_Pawn_INF_Appuyer( const Mission_Pawn_INF_Appuyer& );            //!< Copy constructor
    Mission_Pawn_INF_Appuyer& operator=( const Mission_Pawn_INF_Appuyer& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_INF_Appuyer_h_
