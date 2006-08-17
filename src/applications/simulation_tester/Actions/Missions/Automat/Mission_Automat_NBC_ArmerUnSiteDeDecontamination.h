// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_Automat_NBC_ArmerUnSiteDeDecontamination_h_
#define __Mission_Automat_NBC_ArmerUnSiteDeDecontamination_h_

#include "Actions/Missions/Mission_Automat_ABC.h"

namespace TEST {

    class Automat;

// =============================================================================
/** @class  Mission_Automat_NBC_ArmerUnSiteDeDecontamination
    @brief  Mission_Automat_NBC_ArmerUnSiteDeDecontamination
*/
// Created: SBO 2005-08-08
// Generated: 
// =============================================================================
class Mission_Automat_NBC_ArmerUnSiteDeDecontamination : public Mission_Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_NBC_ArmerUnSiteDeDecontamination( Automat& target );
    virtual ~Mission_Automat_NBC_ArmerUnSiteDeDecontamination();
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
    Mission_Automat_NBC_ArmerUnSiteDeDecontamination( const Mission_Automat_NBC_ArmerUnSiteDeDecontamination& );            //!< Copy constructor
    Mission_Automat_NBC_ArmerUnSiteDeDecontamination& operator=( const Mission_Automat_NBC_ArmerUnSiteDeDecontamination& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Automat_NBC_ArmerUnSiteDeDecontamination_h_
