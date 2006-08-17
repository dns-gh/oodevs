// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Population_Manifester_h_
#define __Mission_Population_Manifester_h_

#include "Actions/Missions/Mission_Population_ABC.h"

namespace TEST {

    class Population;

// =============================================================================
/** @class  Mission_Population_Manifester
    @brief  Mission_Population_Manifester
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Population_Manifester : public Mission_Population_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Population_Manifester( Population& target );
    virtual ~Mission_Population_Manifester();
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
    Mission_Population_Manifester( const Mission_Population_Manifester& );            //!< Copy constructor
    Mission_Population_Manifester& operator=( const Mission_Population_Manifester& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Population_Manifester_h_
