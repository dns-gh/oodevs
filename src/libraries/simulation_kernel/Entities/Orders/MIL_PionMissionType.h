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
    static const MIL_PionMissionType*  Find         ( uint nID );
    static const MIL_PionMissionType*  Find         ( const std::string& strName );
    static const MIL_PionMissionType*  FindFromDiaID( uint nID );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetDIABehavior() const;
    //@}

    //! @name Operations
    //@{
    virtual bool IsAvailableForModel( const DIA_Model& model ) const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             MIL_PionMissionType( uint nID, xml::xistream& xis );
    virtual ~MIL_PionMissionType();
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadMission( xml::xistream& xis );
    //@}

private:
    std::string strDIABehavior_;

private:
    //! @name Types
    //@{
    typedef std::map< uint, const MIL_PionMissionType* > T_MissionIDMap;
    typedef T_MissionIDMap::const_iterator               CIT_MissionIDMap;

    typedef std::map< std::string, const MIL_PionMissionType*, sCaseInsensitiveLess > T_MissionNameMap;
    typedef T_MissionNameMap::const_iterator                                          CIT_MissionNameMap;
    //@}

private:
    static T_MissionIDMap   missionIDs_;
    static T_MissionIDMap   missionDiaIDs_;
    static T_MissionNameMap missionNames_;
};

#include "MIL_PionMissionType.inl"

#endif // __MIL_PionMissionType_h_
