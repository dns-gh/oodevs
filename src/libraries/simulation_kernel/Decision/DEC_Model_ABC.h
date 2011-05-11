// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_Model_ABC.h $
// $Author: Nld $
// $Modtime: 11/04/05 14:37 $
// $Revision: 3 $
// $Workfile: DEC_Model_ABC.h $
//
// *****************************************************************************

#ifndef __DEC_Model_ABC_h_
#define __DEC_Model_ABC_h_

#include "MIL.h"
#include "MT_Tools/MT_String.h"
#include <set>

namespace xml
{
    class xistream;
}

class MIL_MissionType_ABC;
class MIL_FragOrderType;

// =============================================================================
// Created: NLD 2003-11-24
// =============================================================================
class DEC_Model_ABC : private boost::noncopyable
{
public:
    //! @name Accessors
    //@{
    const std::string& GetName() const;
    const std::string& GetScriptFile() const;
    const std::string& GetIncludePath() const;
    const std::string& GetDIAType() const;
    const bool IsMasalife() const;
    //@}

    //! @name Operations
    //@{
    bool IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const;
    bool IsFragOrderAvailableForMission( const MIL_MissionType_ABC& missionType, const MIL_FragOrderType& fragOrderType ) const;
    bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;
    //@}

    //! @name Destructor
    //@{
    virtual ~DEC_Model_ABC();
    //@}

protected:
    //! @name Constructors
    //@{
    DEC_Model_ABC( const std::string& strModel, xml::xistream& xis, const std::string& strSourcePath, const std::map< std::string, const MIL_MissionType_ABC* >& missionTypes, bool isMasalife );
    //@}

private:
    //! @name Initialization tools
    //@{
    static bool FileChanged( const std::string& strFileName, time_t since );
    //@}

    //! @name Initialization
    //@{
    void InitializeMissions( xml::xistream& xis, const std::map< std::string, const MIL_MissionType_ABC* >& missionTypes );
    void InitializeMissionFragOrders( xml::xistream& xis, const MIL_MissionType_ABC& missionType );
    //@}

    //! @name Helpers
    //@{
    void ReadMission( xml::xistream& xis, const std::map< std::string, const MIL_MissionType_ABC* >& missionTypes );
    void ReadFragOrder( xml::xistream& xis, const MIL_MissionType_ABC& missionType );
    void ReadFragOrder( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< const MIL_MissionType_ABC* > T_MissionSet;
    typedef std::set< const MIL_FragOrderType* > T_FragOrderSet;
    typedef std::map< const MIL_MissionType_ABC*, T_FragOrderSet > T_FragOrderPerMissionMap;
    typedef T_FragOrderPerMissionMap::const_iterator             CIT_FragOrderPerMissionMap;
    //@}

private:
    //! @name Member data
    //@{
    const std::string strModel_;
    const bool isMasalife_;
    std::string strDIAType_;
    std::string strScript_;
    std::string strIncludePath_;
    T_MissionSet availableMissions_;
    T_FragOrderSet availableFragOrders_;
    T_FragOrderPerMissionMap availableFragOrdersPerMission_;
    //@}
};

#endif // __DEC_Model_ABC_h_
