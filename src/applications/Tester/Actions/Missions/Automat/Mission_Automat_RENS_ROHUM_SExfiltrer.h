// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Automat_RENS_ROHUM_SExfiltrer_h_
#define __Mission_Automat_RENS_ROHUM_SExfiltrer_h_

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_RENS_ROHUM_SExfiltrer
    @brief  Mission_Automat_RENS_ROHUM_SExfiltrer
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Automat_RENS_ROHUM_SExfiltrer : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_RENS_ROHUM_SExfiltrer( Automat& target );
    virtual ~Mission_Automat_RENS_ROHUM_SExfiltrer();
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
    Mission_Automat_RENS_ROHUM_SExfiltrer( const Mission_Automat_RENS_ROHUM_SExfiltrer& );            //!< Copy constructor
    Mission_Automat_RENS_ROHUM_SExfiltrer& operator=( const Mission_Automat_RENS_ROHUM_SExfiltrer& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_RENS_ROHUM_SExfiltrer_h_
