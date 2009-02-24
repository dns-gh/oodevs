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

namespace xml
{
    class xistream;
}

class DIA_Model;
class DEC_Workspace;
class MIL_MissionType_ABC;
class MIL_FragOrderType;

// =============================================================================
// Created: NLD 2003-11-24
// =============================================================================
class DEC_Model_ABC
{
    MT_COPYNOTALLOWED( DEC_Model_ABC );

public:
    //! @name Accessors
    //@{
    const std::string& GetName      () const;
    const std::string& GetScriptName() const;
          DIA_Model&   GetDIAModel  () const;
    const DIA_Type&    GetDIAType   () const;
    //@}

    //! @name Operations
    //@{
    bool IsMissionAvailable            ( const MIL_MissionType_ABC& missionType ) const;
    bool IsFragOrderAvailableForMission( const MIL_MissionType_ABC& missionType, const MIL_FragOrderType& fragOrderType ) const;
    //@}
    
    //! @name Destructor
    //@{
    virtual ~DEC_Model_ABC();
    //@}

protected:
    //! @name Constructors
    //@{
    DEC_Model_ABC( const DEC_Workspace& decWorkspace, const std::string& strModel, xml::xistream& xis, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strBinaryPath, const std::string& strPrefix, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes );
    //@}

private:
    //! @name Initialization tools
    //@{
           bool NeedScriptParsing( bool bNeedParsing, const std::string& strArchiveName, const std::string& strFileName, const std::string& strOpenedFileArchiveName ) const;
    static bool FileChanged      ( const std::string& strFileName, time_t since );
    //@}

    //! @name Initialization
    //@{
    void InitializeModel            ( const DEC_Workspace& decWorkspace, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strBinaryPath, const std::string& strPrefix );
    void InitializeMissions         ( xml::xistream& xis, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes );
    void InitializeMissionFragOrders( xml::xistream& xis, const MIL_MissionType_ABC& missionType );
    //@}

    //! @name Helpers
    //@{
    void ReadMission  ( xml::xistream& xis, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes );
    void ReadFragOrder( xml::xistream& xis, const MIL_MissionType_ABC& missionType );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< const MIL_MissionType_ABC* >					        T_MissionSet ; 
	typedef std::set< const MIL_FragOrderType* >					        T_FragOrderSet ;
    typedef std::map< const MIL_MissionType_ABC*, T_FragOrderSet > T_FragOrderPerMissionMap;
    typedef T_FragOrderPerMissionMap::const_iterator               CIT_FragOrderPerMissionMap;
    //@}

private:
    const std::string     strModel_;
    const DIA_Type*       pDIAType_;
          DIA_Model*      pDIAModel_;
          std::string     strScript_;
          
		  T_MissionSet				availableMissions_ ; 		  		
		  T_FragOrderPerMissionMap  availableFragOrdersPerMission_;
};

#endif // __DEC_Model_ABC_h_
