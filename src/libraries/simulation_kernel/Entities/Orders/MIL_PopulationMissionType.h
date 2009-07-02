// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PopulationMissionType_h_
#define __MIL_PopulationMissionType_h_

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
    static void                       Initialize( xml::xistream& xis );
    static const MIL_MissionType_ABC* Find      ( uint nID );
    static const MIL_MissionType_ABC* Find      ( const std::string& strName );
    static T_MissionNameMap&          MissionNames();
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
    friend class MIL_MissionType_ABC;
    MIL_PopulationMissionType( uint nID, xml::xistream& xis );
    virtual ~MIL_PopulationMissionType();
    //@}

    //! @name Heplers
    //@{
    struct LoadingWrapper;
    //@}

private:
    std::string strDIABehavior_;

private:
    static T_MissionIDMap   missionIDs_;
    static T_MissionNameMap missionNames_;
};

#endif // __MIL_PopulationMissionType_h_
