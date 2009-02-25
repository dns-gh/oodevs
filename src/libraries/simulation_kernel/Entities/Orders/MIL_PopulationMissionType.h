// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PopulationMissionType_h_
#define __MIL_PopulationMissionType_h_

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
class MIL_PopulationMissionType : public MIL_MissionType_ABC
{
public:
    //! @name Factory
    //@{
    static void                             Initialize( xml::xistream& xis );
    static const MIL_PopulationMissionType* Find      ( uint nID );
    static const MIL_MissionType_ABC*       Find      ( const std::string& strName );
    static T_MissionNameMap& MissionNames();
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
    //! @name Constructors/Destructor
    //@{
    MIL_PopulationMissionType( uint nID, xml::xistream& xis );
    virtual ~MIL_PopulationMissionType();
    //@}

    //! @name Heplers
    //@{
    struct LoadingWrapper;
    static void ReadMission( xml::xistream& xis );
    //@}

private:
    std::string strDIABehavior_;

private:
    //! @name Types
    //@{
    typedef std::map< uint, const MIL_PopulationMissionType* > T_MissionIDMap;
    typedef T_MissionIDMap::const_iterator                     CIT_MissionIDMap;
    //@}

private:
    static T_MissionIDMap   missionIDs_;
    static T_MissionNameMap missionNames_;
};

#include "MIL_PopulationMissionType.inl"

#endif // __MIL_PopulationMissionType_h_
