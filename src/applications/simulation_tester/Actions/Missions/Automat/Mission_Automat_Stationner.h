// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Automat_Stationner_h_
#define __Mission_Automat_Stationner_h_

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_Stationner
    @brief  Mission_Automat_Stationner
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Automat_Stationner : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_Stationner( Automat& target );
    virtual ~Mission_Automat_Stationner();
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
    Mission_Automat_Stationner( const Mission_Automat_Stationner& );            //!< Copy constructor
    Mission_Automat_Stationner& operator=( const Mission_Automat_Stationner& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_Stationner_h_
