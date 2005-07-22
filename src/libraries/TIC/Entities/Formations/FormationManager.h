// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Formations/FormationManager.h $
// $Author: Sbo $
// $Modtime: 21/07/05 17:17 $
// $Revision: 2 $
// $Workfile: FormationManager.h $
//
// *****************************************************************************

#ifndef __FormationManager_h_
#define __FormationManager_h_

#include "Types.h"

namespace TIC
{
    class Pawn;
    class Formation_ABC;

// =============================================================================
/** @class  FormationManager
    @brief  FormationManager
    @par    Using example
    @code
    FormationManager;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class FormationManager
{
public:
    //! @name enum
    //@{
    enum EFormation
    {
        eFormationRoad,
        eFormationLine,
        eFormationDiamond
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             FormationManager();
    virtual ~FormationManager();
    //@}

    //! @name Static Operations
    //@{
    static void           Initialize  ();
    static void           Terminate   ();
    static Formation_ABC& GetFormation( EFormation eFormation );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FormationManager( const FormationManager& );            //!< Copy constructor
    FormationManager& operator=( const FormationManager& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< EFormation, Formation_ABC* > T_FormationMap;
    typedef T_FormationMap::const_iterator         CIT_FormationMap;
    //@}

private:
    //! @name Static Members
    //@{
    static T_FormationMap formationMap_;
    //@}
};

} // end namespace TIC

#include "FormationManager.inl"


#endif // __FormationManager_h_
