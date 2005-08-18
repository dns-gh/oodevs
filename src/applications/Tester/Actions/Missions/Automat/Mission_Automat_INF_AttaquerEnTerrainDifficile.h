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

#ifndef __Mission_Automat_INF_AttaquerEnTerrainDifficile_h_
#define __Mission_Automat_INF_AttaquerEnTerrainDifficile_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_INF_AttaquerEnTerrainDifficile
    @brief  Mission_Automat_INF_AttaquerEnTerrainDifficile
    @par    Using example
    @code
    Mission_Automat_INF_AttaquerEnTerrainDifficile;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-17 - 16:32:40
// =============================================================================
class Mission_Automat_INF_AttaquerEnTerrainDifficile : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_INF_AttaquerEnTerrainDifficile( Automat& target );
    virtual ~Mission_Automat_INF_AttaquerEnTerrainDifficile();
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
    Mission_Automat_INF_AttaquerEnTerrainDifficile( const Mission_Automat_INF_AttaquerEnTerrainDifficile& );            //!< Copy constructor
    Mission_Automat_INF_AttaquerEnTerrainDifficile& operator=( const Mission_Automat_INF_AttaquerEnTerrainDifficile& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_INF_AttaquerEnTerrainDifficile_h_
