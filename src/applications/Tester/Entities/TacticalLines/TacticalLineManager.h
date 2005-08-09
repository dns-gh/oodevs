// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-09 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __TacticalLineManager_h_
#define __TacticalLineManager_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

namespace TEST {

    class TacticalLine_ABC;

// =============================================================================
/** @class  TacticalLineManager
    @brief  TacticalLineManager
    @par    Using example
    @code
    TacticalLineManager;
    @endcode
*/
// Created: SBO 2005-08-09
// =============================================================================
class TacticalLineManager
{

public:
    //! @name Static operations
    //@{
    static void              Initialize         ();
    static void              Terminate          ();
    static void              Register           ( TacticalLine_ABC& line );
    static void              UnRegister         ( TacticalLine_ABC& line );
    static TacticalLine_ABC* Find               ( T_EntityId        nId  );
    static T_EntityId        GetLimitIdExcluding( T_EntityId        nId  );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             TacticalLineManager();
    virtual ~TacticalLineManager();
    //@}

    //! @name Copy/Assignment
    //@{
    TacticalLineManager( const TacticalLineManager& );            //!< Copy constructor
    TacticalLineManager& operator=( const TacticalLineManager& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< TacticalLine_ABC* >     T_TacticalLineSet;
    typedef T_TacticalLineSet::iterator       IT_TacticalLineSet;
    typedef T_TacticalLineSet::const_iterator CIT_TacticalLineSet;
    //@}

private:
    //! @name Member data
    //@{
    static T_TacticalLineSet lines_;
    //@}
};

} // end namespace TEST

#endif // __TacticalLineManager_h_
