// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Mission_Pawn_Test_Heliporter_h_
#define __Mission_Pawn_Test_Heliporter_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_Test_Heliporter
    @brief  Mission_Pawn_Test_Heliporter
    @par    Using example
    @code
    Mission_Pawn_Test_Heliporter;
    @endcode
*/
// Created: SBO 2005-08-08
// =============================================================================
class Mission_Pawn_Test_Heliporter : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_Test_Heliporter( Pawn& target, uint nExecutionTick = 0 );
    virtual ~Mission_Pawn_Test_Heliporter();
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
    Mission_Pawn_Test_Heliporter( const Mission_Pawn_Test_Heliporter& );            //!< Copy constructor
    Mission_Pawn_Test_Heliporter& operator=( const Mission_Pawn_Test_Heliporter& ); //!< Assignment operator
    //@}
};

} // end namespace Mission


#endif // __Mission_Pawn_Test_Heliporter_h_
