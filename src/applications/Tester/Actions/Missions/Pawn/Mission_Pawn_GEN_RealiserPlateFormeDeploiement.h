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

#ifndef __Mission_Pawn_GEN_RealiserPlateFormeDeploiement_h_
#define __Mission_Pawn_GEN_RealiserPlateFormeDeploiement_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_GEN_RealiserPlateFormeDeploiement
    @brief  Mission_Pawn_GEN_RealiserPlateFormeDeploiement
    @par    Using example
    @code
    Mission_Pawn_GEN_RealiserPlateFormeDeploiement;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-23 - 19:03:04
// =============================================================================
class Mission_Pawn_GEN_RealiserPlateFormeDeploiement : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_GEN_RealiserPlateFormeDeploiement( Pawn& target );
    virtual ~Mission_Pawn_GEN_RealiserPlateFormeDeploiement();
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
    Mission_Pawn_GEN_RealiserPlateFormeDeploiement( const Mission_Pawn_GEN_RealiserPlateFormeDeploiement& );            //!< Copy constructor
    Mission_Pawn_GEN_RealiserPlateFormeDeploiement& operator=( const Mission_Pawn_GEN_RealiserPlateFormeDeploiement& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_GEN_RealiserPlateFormeDeploiement_h_
