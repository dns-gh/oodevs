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

class DEC_ModelPion;
class DEC_ModelAutomate;
class DEC_ModelPopulation;
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
    const DEC_ModelPion*       FindModelPion         ( const std::string& strModelName  ) const;
    const DEC_ModelAutomate*   FindModelAutomate     ( const std::string& strModelName  ) const;
    const DEC_ModelPopulation* FindModelPopulation   ( const std::string& strModelName  ) const;    
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
    typedef std::map< std::string, const DEC_ModelPion*, sCaseInsensitiveLess > T_ModelPionMap;
    typedef T_ModelPionMap::const_iterator                                      CIT_ModelPionMap;

    typedef std::map< std::string, const DEC_ModelAutomate*, sCaseInsensitiveLess > T_ModelAutomateMap;
    typedef T_ModelAutomateMap::const_iterator                                      CIT_ModelAutomateMap;

    typedef std::map< std::string, const DEC_ModelPopulation*, sCaseInsensitiveLess > T_ModelPopulationMap;
    typedef T_ModelPopulationMap::const_iterator                                      CIT_ModelPopulationMap;
    //@}

    //! @name Helpers
    //@{
    void ReadUnit      ( xml::xistream& xis, bool bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath );
    void ReadAutomat   ( xml::xistream& xis, bool bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath );
    void ReadPopulation( xml::xistream& xis, bool bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath );
    //@}

private:
    DEC_Debug                           debug_;
    DIA_FunctionTable< DEC_Workspace >* pFuncTable_;
    DIA_FunctionCaller_ABC*             pFunctionCaller_;

    T_ModelPionMap                      pionModels_;
    T_ModelAutomateMap                  automateModels_;
    T_ModelPopulationMap                populationModels_;
};

#include "DEC_Workspace.inl"

#endif // __DEC_Workspace_h_
