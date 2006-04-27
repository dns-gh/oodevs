// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Automat_ALAT_Heliporter_h_
#define __Mission_Automat_ALAT_Heliporter_h_

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_ALAT_Heliporter
    @brief  Mission_Automat_ALAT_Heliporter
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Automat_ALAT_Heliporter : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_ALAT_Heliporter( Automat& target );
    virtual ~Mission_Automat_ALAT_Heliporter();
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
    Mission_Automat_ALAT_Heliporter( const Mission_Automat_ALAT_Heliporter& );            //!< Copy constructor
    Mission_Automat_ALAT_Heliporter& operator=( const Mission_Automat_ALAT_Heliporter& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_ALAT_Heliporter_h_
