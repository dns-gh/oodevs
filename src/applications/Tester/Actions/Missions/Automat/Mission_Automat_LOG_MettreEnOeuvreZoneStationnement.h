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

#ifndef __Mission_Automat_LOG_MettreEnOeuvreZoneStationnement_h_
#define __Mission_Automat_LOG_MettreEnOeuvreZoneStationnement_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_LOG_MettreEnOeuvreZoneStationnement
    @brief  Mission_Automat_LOG_MettreEnOeuvreZoneStationnement
    @par    Using example
    @code
    Mission_Automat_LOG_MettreEnOeuvreZoneStationnement;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-17 - 16:32:40
// =============================================================================
class Mission_Automat_LOG_MettreEnOeuvreZoneStationnement : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_LOG_MettreEnOeuvreZoneStationnement( Automat& target );
    virtual ~Mission_Automat_LOG_MettreEnOeuvreZoneStationnement();
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
    Mission_Automat_LOG_MettreEnOeuvreZoneStationnement( const Mission_Automat_LOG_MettreEnOeuvreZoneStationnement& );            //!< Copy constructor
    Mission_Automat_LOG_MettreEnOeuvreZoneStationnement& operator=( const Mission_Automat_LOG_MettreEnOeuvreZoneStationnement& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_LOG_MettreEnOeuvreZoneStationnement_h_
