// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PionMissionType_h_
#define __MIL_PionMissionType_h_

#include "MIL.h"

#include "MIL_MissionType_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  MIL_MissionType_ABC
    @brief  MIL_MissionType_ABC
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_PionMissionType : public MIL_MissionType_ABC
{
public:
    //! @name Factory
    //@{
    static void                        Initialize   ( xml::xistream& xis );
    static const MIL_MissionType_ABC*  Find         ( uint nID );
    static const MIL_MissionType_ABC*  Find         ( const std::string& strName );
    static const MIL_MissionType_ABC*  FindFromDiaID( uint nID );
    static const T_MissionNameMap& MissionNames();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetDIABehavior( Phase ) const;
    //@}

    //! @name Operations
    //@{
    virtual bool IsAvailableForModel( const DIA_Model& model ) const;
    //@}

private:
    friend class MIL_MissionType_ABC;
    //! @name Constructors/Destructor
    //@{
             MIL_PionMissionType( uint nID, xml::xistream& xis );
    virtual ~MIL_PionMissionType();
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    //@}

private:
    std::string strDIABehavior_;

private:
    static T_MissionIDMap   missionIDs_;
    static T_MissionIDMap   missionDiaIDs_;
    static T_MissionNameMap missionNames_;
};

#include "MIL_PionMissionType.inl"

#endif // __MIL_PionMissionType_h_
