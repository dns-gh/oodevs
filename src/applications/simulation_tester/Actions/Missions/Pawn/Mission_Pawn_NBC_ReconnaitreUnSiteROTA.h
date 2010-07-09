// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_NBC_ReconnaitreUnSiteROTA_h_
#define __Mission_Pawn_NBC_ReconnaitreUnSiteROTA_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_NBC_ReconnaitreUnSiteROTA
    @brief  Mission_Pawn_NBC_ReconnaitreUnSiteROTA
*/
// Created: SBO 2005-08-08
// Generated:
// =============================================================================
class Mission_Pawn_NBC_ReconnaitreUnSiteROTA : public Mission_Pawn_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_NBC_ReconnaitreUnSiteROTA( Pawn& target );
    virtual ~Mission_Pawn_NBC_ReconnaitreUnSiteROTA();
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
    Mission_Pawn_NBC_ReconnaitreUnSiteROTA( const Mission_Pawn_NBC_ReconnaitreUnSiteROTA& );            //!< Copy constructor
    Mission_Pawn_NBC_ReconnaitreUnSiteROTA& operator=( const Mission_Pawn_NBC_ReconnaitreUnSiteROTA& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_NBC_ReconnaitreUnSiteROTA_h_
