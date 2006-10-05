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

#include "Types.h"
#include "Messages/ASN_Messages.h"

namespace TEST {

    class TacticalLine_ABC;
    class PositionManager;

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
    MT_COPYNOTALLOWED( TacticalLineManager );

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLineManager( const PositionManager& posMgr );
    virtual ~TacticalLineManager();
    //@}

    //! @name Operations
    //@{
    void              LoadTacticalLines( const std::string& strConfigFile );
    void              Register         ( TacticalLine_ABC&  line          );
    void              UnRegister       ( TacticalLine_ABC&  line          );
    TacticalLine_ABC* Find             ( T_EntityId         nId           );
    T_EntityId        GetNextLimitId   ();
    T_EntityId        GetLimaByType    ( ASN1T_EnumTypeLima eLimaType     );
    void              UpdateToSim      ();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDefaultTacticalLines( const PositionManager& posMgr );
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
    T_TacticalLineSet lines_;
    //@}
};

} // end namespace TEST

#endif // __TacticalLineManager_h_
