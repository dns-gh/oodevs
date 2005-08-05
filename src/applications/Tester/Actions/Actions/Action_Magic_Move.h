// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

#ifndef __Action_Magic_Move_h_
#define __Action_Magic_Move_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Action.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Action_Magic_Move
    @brief  Action_Magic_Move
    @par    Using example
    @code
    Action_Magic_Move;
    @endcode
*/
// Created: SBO 2005-08-04
// =============================================================================
class Action_Magic_Move : public Action< Pawn >
{

public:
    //! @name Constructors/Destructor
    //@{
             Action_Magic_Move( Pawn& target, uint nExecutionTick = 0 );
    virtual ~Action_Magic_Move();
    //@}

protected:
    //! @name Action validation and commit
    //@{
    virtual bool Serialize() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Action_Magic_Move( const Action_Magic_Move& );            //!< Copy constructor
    Action_Magic_Move& operator=( const Action_Magic_Move& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Action_Magic_Move_h_
