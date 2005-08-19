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

#ifndef __Mission_Automat_SeFaireTransporter_h_
#define __Mission_Automat_SeFaireTransporter_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_SeFaireTransporter
    @brief  Mission_Automat_SeFaireTransporter
    @par    Using example
    @code
    Mission_Automat_SeFaireTransporter;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-19 - 11:31:08
// =============================================================================
class Mission_Automat_SeFaireTransporter : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_SeFaireTransporter( Automat& target );
    virtual ~Mission_Automat_SeFaireTransporter();
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
    Mission_Automat_SeFaireTransporter( const Mission_Automat_SeFaireTransporter& );            //!< Copy constructor
    Mission_Automat_SeFaireTransporter& operator=( const Mission_Automat_SeFaireTransporter& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_SeFaireTransporter_h_
