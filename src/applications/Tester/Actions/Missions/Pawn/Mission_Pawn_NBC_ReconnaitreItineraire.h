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

#ifndef __Mission_Pawn_NBC_ReconnaitreItineraire_h_
#define __Mission_Pawn_NBC_ReconnaitreItineraire_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_NBC_ReconnaitreItineraire
    @brief  Mission_Pawn_NBC_ReconnaitreItineraire
    @par    Using example
    @code
    Mission_Pawn_NBC_ReconnaitreItineraire;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-26 - 18:35:00
// =============================================================================
class Mission_Pawn_NBC_ReconnaitreItineraire : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_NBC_ReconnaitreItineraire( Pawn& target );
    virtual ~Mission_Pawn_NBC_ReconnaitreItineraire();
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
    Mission_Pawn_NBC_ReconnaitreItineraire( const Mission_Pawn_NBC_ReconnaitreItineraire& );            //!< Copy constructor
    Mission_Pawn_NBC_ReconnaitreItineraire& operator=( const Mission_Pawn_NBC_ReconnaitreItineraire& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_NBC_ReconnaitreItineraire_h_
