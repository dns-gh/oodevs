// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Population_FaireMouvement_h_
#define __Mission_Population_FaireMouvement_h_

#include "Actions/Missions/Mission_Population_ABC.h"

namespace TEST {

    class Population;

// =============================================================================
/** @class  Mission_Population_FaireMouvement
    @brief  Mission_Population_FaireMouvement
*/
// Created: SBO 2005-08-08
// Generated:
// =============================================================================
class Mission_Population_FaireMouvement : public Mission_Population_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission_Population_FaireMouvement( Population& target );
    virtual ~Mission_Population_FaireMouvement();
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
    Mission_Population_FaireMouvement( const Mission_Population_FaireMouvement& );            //!< Copy constructor
    Mission_Population_FaireMouvement& operator=( const Mission_Population_FaireMouvement& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Population_FaireMouvement_h_