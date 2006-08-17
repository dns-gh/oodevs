// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Population_CommettreExactions_h_
#define __Mission_Population_CommettreExactions_h_

#include "Actions/Missions/Mission_Population_ABC.h"

namespace TEST {

    class Population;

// =============================================================================
/** @class  Mission_Population_CommettreExactions
    @brief  Mission_Population_CommettreExactions
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Population_CommettreExactions : public Mission_Population_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Population_CommettreExactions( Population& target );
    virtual ~Mission_Population_CommettreExactions();
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
    Mission_Population_CommettreExactions( const Mission_Population_CommettreExactions& );            //!< Copy constructor
    Mission_Population_CommettreExactions& operator=( const Mission_Population_CommettreExactions& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Population_CommettreExactions_h_
