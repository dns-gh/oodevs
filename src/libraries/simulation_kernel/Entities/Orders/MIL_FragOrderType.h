// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_FragOrderType_h_
#define __MIL_FragOrderType_h_

#include "MIL_OrderType_ABC.h"
#include "MT_Tools/MT_String.h"

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
class MIL_FragOrderType : public MIL_OrderType_ABC
{
public:
    //! @name Factory
    //@{
    static void                      Initialize( xml::xistream& xis );
    static const MIL_FragOrderType*  Find      ( unsigned int nID );
    static const MIL_FragOrderType*  Find      ( const std::string& strName );
    static const MIL_FragOrderType*  FindByDiaType( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    bool IsAvailableWithoutMission() const;
    bool IsAvailableForAllMissions() const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             MIL_FragOrderType( unsigned int nID, xml::xistream& xis );
    virtual ~MIL_FragOrderType();
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadFragorder( xml::xistream& xis );
    //@}

private:
    bool bAvailableWithoutMission_;
    bool bAvailableForAllMissions_;

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, const MIL_FragOrderType* > T_MissionIDMap;
    typedef T_MissionIDMap::const_iterator             CIT_MissionIDMap;

    typedef std::map< std::string, const MIL_FragOrderType* > T_MissionNameMap;
    typedef T_MissionNameMap::const_iterator CIT_MissionNameMap;
    //@}

private:
    static T_MissionIDMap   missionIDs_;
    static T_MissionNameMap missionNames_;
    static T_MissionNameMap fragOrderDiaTypes_;
};

#endif // __MIL_FragOrderType_h_
