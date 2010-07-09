// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Automat_LOG_ReconnaitreZoneContaminee_h_
#define __Mission_Automat_LOG_ReconnaitreZoneContaminee_h_

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_LOG_ReconnaitreZoneContaminee
    @brief  Mission_Automat_LOG_ReconnaitreZoneContaminee
*/
// Created: SBO 2005-08-08
// Generated:
// =============================================================================
class Mission_Automat_LOG_ReconnaitreZoneContaminee : public Mission_Automat_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_LOG_ReconnaitreZoneContaminee( Automat& target );
    virtual ~Mission_Automat_LOG_ReconnaitreZoneContaminee();
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
    Mission_Automat_LOG_ReconnaitreZoneContaminee( const Mission_Automat_LOG_ReconnaitreZoneContaminee& );            //!< Copy constructor
    Mission_Automat_LOG_ReconnaitreZoneContaminee& operator=( const Mission_Automat_LOG_ReconnaitreZoneContaminee& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_LOG_ReconnaitreZoneContaminee_h_
