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

#ifndef __Mission_Pawn_INF_ContreAttaquer_h_
#define __Mission_Pawn_INF_ContreAttaquer_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_INF_ContreAttaquer
    @brief  Mission_Pawn_INF_ContreAttaquer
    @par    Using example
    @code
    Mission_Pawn_INF_ContreAttaquer;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-11 - 11:53:20
// =============================================================================
class Mission_Pawn_INF_ContreAttaquer : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_INF_ContreAttaquer( Pawn& target, uint nExecutionTick = 0 );
    virtual ~Mission_Pawn_INF_ContreAttaquer();
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
    Mission_Pawn_INF_ContreAttaquer( const Mission_Pawn_INF_ContreAttaquer& );            //!< Copy constructor
    Mission_Pawn_INF_ContreAttaquer& operator=( const Mission_Pawn_INF_ContreAttaquer& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_INF_ContreAttaquer_h_
