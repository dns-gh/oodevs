//*****************************************************************************
// Created: JVT 02-06-28
//*****************************************************************************

#ifndef __DEC_Workspace_h_
#define __DEC_Workspace_h_

#include "MIL.h"

#include "DEC_Debug.h"

namespace xml
{
    class xistream;
}

class DEC_Model_ABC;
class MIL_MissionType_ABC;
class MIL_Config;

//*****************************************************************************
// Created: JVT 02-06-28
// Last modified: AGN 02-11-05
//*****************************************************************************
class DEC_Workspace
{
    MT_COPYNOTALLOWED( DEC_Workspace )
     
public:
     DEC_Workspace( MIL_Config& config );
    ~DEC_Workspace();

    //! @name Fonction dia
    //@{
    virtual float GetTime() const;
    //@}

    //! @name Models management
    //@{
          DIA_Model*           FindDIAModelFromScript( const std::string& strScriptName ) const;
    const DEC_Model_ABC*       FindModel             ( const std::string& strModelName  ) const;    
    //@}

    //! @name Accessors
    //@{
    DEC_Debug& GetDebug();
    //@}  
    
private:
    //! @name Init
    //@{
    static void RegisterDIA_Functions( DIA_FunctionTable< DEC_Workspace >* pTable );
    static bool CheckFilesDepencies  ( const std::string& strArchiveFile );

    void InitializeConfig      ( MIL_Config& config );
    void InitializeMissions    ( MIL_Config& config );
    void InitializeDIA         ( MIL_Config& config );
    void InitializeDIATypes    ( xml::xistream& xis, bool& bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath );
    void InitializeDIAWorkspace( xml::xistream& xis, bool& bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath );
    void InitializeModels      ( MIL_Config& config, bool bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath );   
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const DEC_Model_ABC*, sCaseInsensitiveLess > T_ModelMap;
    typedef T_ModelMap::const_iterator                                          CIT_ModelMap;
    typedef std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > T_MissionTypeNameMap;
    //@}

    //! @name Helpers
    //@{
    void ReadModel( xml::xistream& xis, bool bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath, const std::string& strEntityType, const T_MissionTypeNameMap& missionTypes );
    //@}

private:
    DEC_Debug                           debug_;
    DIA_FunctionTable< DEC_Workspace >* pFuncTable_;
    DIA_FunctionCaller_ABC*             pFunctionCaller_;

    T_ModelMap                          models_;

    T_MissionTypeNameMap      unitMissionsNames_ ; 
    T_MissionTypeNameMap      automatMissionsNames_ ; 
    T_MissionTypeNameMap      populationMissionsNames_ ; 
};

#include "DEC_Workspace.inl"

#endif // __DEC_Workspace_h_
