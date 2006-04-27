// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Pawn_GEN_RealiserCampPrisonniers_h_
#define __Mission_Pawn_GEN_RealiserCampPrisonniers_h_

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_GEN_RealiserCampPrisonniers
    @brief  Mission_Pawn_GEN_RealiserCampPrisonniers
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Pawn_GEN_RealiserCampPrisonniers : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_GEN_RealiserCampPrisonniers( Pawn& target );
    virtual ~Mission_Pawn_GEN_RealiserCampPrisonniers();
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
    Mission_Pawn_GEN_RealiserCampPrisonniers( const Mission_Pawn_GEN_RealiserCampPrisonniers& );            //!< Copy constructor
    Mission_Pawn_GEN_RealiserCampPrisonniers& operator=( const Mission_Pawn_GEN_RealiserCampPrisonniers& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_GEN_RealiserCampPrisonniers_h_
