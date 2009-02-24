// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_MissionType_ABC_h_
#define __MIL_MissionType_ABC_h_

#include "MIL_OrderType_ABC.h"

// =============================================================================
/** @class  MIL_MissionType_ABC
    @brief  MIL_MissionType_ABC
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_MissionType_ABC : public MIL_OrderType_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_MissionType_ABC( uint nID, xml::xistream& xis );
    virtual ~MIL_MissionType_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool IsAvailableForModel( const DIA_Model& model ) const = 0;
    virtual const std::string& GetDIABehavior( int phase = 0 ) const = 0;
    //@}
    
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > T_MissionNameMap;
    typedef T_MissionNameMap::const_iterator                                          CIT_MissionNameMap;
    //@}
};

#endif // __MIL_MissionType_ABC_h_
