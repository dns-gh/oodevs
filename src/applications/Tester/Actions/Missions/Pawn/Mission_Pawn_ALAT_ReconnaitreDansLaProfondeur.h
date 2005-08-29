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

#ifndef __Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur_h_
#define __Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Actions/Missions/Mission_Pawn_ABC.h"

namespace TEST {

    class Pawn;

// =============================================================================
/** @class  Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur
    @brief  Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur
    @par    Using example
    @code
    Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur;
    @endcode
*/
// Created: SBO 2005-08-08
// Generated: 2005-08-26 - 18:34:59
// =============================================================================
class Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur : public Mission_Pawn_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur( Pawn& target );
    virtual ~Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur();
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
    Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur( const Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur& );            //!< Copy constructor
    Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur& operator=( const Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur& ); //!< Assignment operator
    //@}
};

} // end namespace TEST

#endif // __Mission_Pawn_ALAT_ReconnaitreDansLaProfondeur_h_
