// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

#ifndef __Mission_Automat_LOG_ReconnaitreItineraire_h_
#define __Mission_Automat_LOG_ReconnaitreItineraire_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_LOG_ReconnaitreItineraire
    @brief  Mission_Automat_LOG_ReconnaitreItineraire
    @par    Using example
    @code
    Mission_Automat_LOG_ReconnaitreItineraire;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-23 - 19:03:03
// =============================================================================
class Mission_Automat_LOG_ReconnaitreItineraire : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_LOG_ReconnaitreItineraire( Automat& target );
    virtual ~Mission_Automat_LOG_ReconnaitreItineraire();
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
    Mission_Automat_LOG_ReconnaitreItineraire( const Mission_Automat_LOG_ReconnaitreItineraire& );            //!< Copy constructor
    Mission_Automat_LOG_ReconnaitreItineraire& operator=( const Mission_Automat_LOG_ReconnaitreItineraire& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_LOG_ReconnaitreItineraire_h_
