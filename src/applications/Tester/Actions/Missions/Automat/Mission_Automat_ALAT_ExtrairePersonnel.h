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

#ifndef __Mission_Automat_ALAT_ExtrairePersonnel_h_
#define __Mission_Automat_ALAT_ExtrairePersonnel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_ALAT_ExtrairePersonnel
    @brief  Mission_Automat_ALAT_ExtrairePersonnel
    @par    Using example
    @code
    Mission_Automat_ALAT_ExtrairePersonnel;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-23 - 19:03:03
// =============================================================================
class Mission_Automat_ALAT_ExtrairePersonnel : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_ALAT_ExtrairePersonnel( Automat& target );
    virtual ~Mission_Automat_ALAT_ExtrairePersonnel();
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
    Mission_Automat_ALAT_ExtrairePersonnel( const Mission_Automat_ALAT_ExtrairePersonnel& );            //!< Copy constructor
    Mission_Automat_ALAT_ExtrairePersonnel& operator=( const Mission_Automat_ALAT_ExtrairePersonnel& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_ALAT_ExtrairePersonnel_h_
