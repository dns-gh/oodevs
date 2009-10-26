//*****************************************************************************
// Created: JVT 02-06-28
//*****************************************************************************

#ifndef __DEC_Workspace_h_
#define __DEC_Workspace_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class DEC_Model_ABC;
class DEC_DataBase;
class MIL_MissionType_ABC;
class MIL_Config;

namespace directia
{
    class Brain;
}


//*****************************************************************************
// Created: JVT 02-06-28
// Last modified: AGN 02-11-05
//*****************************************************************************
class DEC_Workspace : private boost::noncopyable
{
     
public:
     DEC_Workspace( MIL_Config& config );
    ~DEC_Workspace();

    //! @name Fonction dia
    //@{
    virtual float GetTime() const;
    //@}

    //! @name Models management
    //@{
    const DEC_Model_ABC*       FindModelPion         ( const std::string& strModelName  ) const;  
    const DEC_Model_ABC*       FindModelAutomate     ( const std::string& strModelName  ) const;  
    const DEC_Model_ABC*       FindModelPopulation   ( const std::string& strModelName  ) const;    
    //@}

    void InitKnowledges( directia::Brain& brain ) const;
    
private:
    //! @name Init
    //@{
    void InitializeConfig      ( MIL_Config& config );
    void InitializeObjectNames ( MIL_Config& config );
    void InitializeMissions    ( MIL_Config& config );
    void InitializeDIA         ( MIL_Config& config );
    void InitializeModels      ( MIL_Config& config, const std::string& strSourcePath );   
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const DEC_Model_ABC*, sCaseInsensitiveLess > T_ModelMap;
    typedef T_ModelMap::const_iterator                                          CIT_ModelMap;
    typedef std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess > T_MissionTypeNameMap;
    typedef std::map< std::string, T_ModelMap* >                                T_ModelTypeMap;
    //@}

    //! @name Helpers
    //@{
    void ReadModel( xml::xistream& xis, const std::string& strBinaryPath, const std::string& strEntityType, const T_MissionTypeNameMap& missionTypes );
    //@}

private:
    T_ModelMap                          automateModels_;
    T_ModelMap                          pionModels_;
    T_ModelMap                          populationModels_;
    T_ModelTypeMap                      modelTypes_;

    T_MissionTypeNameMap                unitMissionsNames_ ; 
    T_MissionTypeNameMap                automatMissionsNames_ ; 
    T_MissionTypeNameMap                populationMissionsNames_ ;

    std::auto_ptr< DEC_DataBase > dataBase_;
};

#include "DEC_Workspace.inl"

#endif // __DEC_Workspace_h_
