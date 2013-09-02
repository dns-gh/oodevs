// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_AutomateMissionType_h_
#define __MIL_AutomateMissionType_h_

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
class MIL_AutomateMissionType : public MIL_MissionType_ABC
{
public:
    //! @name Factory
    //@{
    static void                       Initialize   ( xml::xistream& xis );
    static const MIL_MissionType_ABC* Find         ( unsigned int nID );
    static const MIL_MissionType_ABC* Find         ( const std::string& strName );
    static const MIL_MissionType_ABC* FindFromDiaID( const std::string& nID );
    static const T_MissionNameMap&    MissionNames();
    //@}

    //! @name Accessors
    //@{
    virtual const std::string& GetDIABehavior( Phase phase ) const;
    //@}

private:
    friend class MIL_MissionType_ABC;
    //! @name Constructors/Destructor
    //@{
             MIL_AutomateMissionType( unsigned int nID, xml::xistream& xis );
    virtual ~MIL_AutomateMissionType();
    //@}

private:
    std::string strDIAMrtBehavior_;
    std::string strDIACdtBehavior_;

private:
    static T_MissionIDMap   missionIDs_;
    static T_MissionNameMap missionDiaIDs_;
    static T_MissionNameMap missionNames_;
};

#endif // __MIL_AutomateMissionType_h_
