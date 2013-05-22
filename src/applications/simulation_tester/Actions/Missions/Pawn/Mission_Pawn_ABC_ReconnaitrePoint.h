// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_ABC_ReconnaitrePoint_h_
#define __Mission_Pawn_ABC_ReconnaitrePoint_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_ABC_ReconnaitrePoint
    @brief  Mission_Pawn_ABC_ReconnaitrePoint
*/
// Created: SBO 2005-08-08
// Generated:
// =============================================================================
class Mission_Pawn_ABC_ReconnaitrePoint : public Mission_Pawn_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ABC_ReconnaitrePoint( Pawn& target );
    virtual ~Mission_Pawn_ABC_ReconnaitrePoint();
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
    Mission_Pawn_ABC_ReconnaitrePoint( const Mission_Pawn_ABC_ReconnaitrePoint& );            //!< Copy constructor
    Mission_Pawn_ABC_ReconnaitrePoint& operator=( const Mission_Pawn_ABC_ReconnaitrePoint& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ABC_ReconnaitrePoint_h_