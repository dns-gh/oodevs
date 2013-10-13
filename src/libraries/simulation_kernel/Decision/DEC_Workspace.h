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

namespace tools
{
    class Path;
}

class DEC_Model_ABC;
class MIL_MissionType_ABC;
class MIL_Config;

namespace directia
{
namespace brain
{
    class Brain;
}
}


//*****************************************************************************
// Created: JVT 02-06-28
// Last modified: AGN 02-11-05
//*****************************************************************************
class DEC_Workspace : private boost::noncopyable
{
public:
    explicit DEC_Workspace( MIL_Config& config );
    virtual ~DEC_Workspace();

    //! @name Fonction dia
    //@{
    virtual float GetTime() const;
    //@}

    //! @name Models management
    //@{
    const DEC_Model_ABC* FindModelPion( const std::string& strModelName  ) const;
    const DEC_Model_ABC* FindModelAutomate( const std::string& strModelName  ) const;
    const DEC_Model_ABC* FindModelPopulation( const std::string& strModelName  ) const;
    //@}

private:
    //! @name Init
    //@{
    void InitializeConfig      ( MIL_Config& config );
    void InitializeObjectNames ( MIL_Config& config );
    void InitializeMissions    ( MIL_Config& config );
    void InitializeDIA         ( MIL_Config& config );
    void InitializeModels      ( MIL_Config& config, const std::map< std::string, tools::Path >& strSourcePaths );
    void LoadDIA               ( MIL_Config& config, xml::xistream& xis );
    void LoadDecisional        ( xml::xistream& xis, unsigned int tickDuration );
    void LoadMissions          ( xml::xistream& xis );
    void LoadModels            ( xml::xistream& xis, const std::map< std::string, tools::Path >& strSourcePaths,
                                 const tools::Path& integrationDir );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const DEC_Model_ABC* > T_ModelMap;
    typedef T_ModelMap::const_iterator                  CIT_ModelMap;
    typedef std::map< std::string, const MIL_MissionType_ABC* > T_MissionTypeNameMap;
    typedef std::map< std::string, T_ModelMap* >                T_ModelTypeMap;
    //@}

    //! @name Helpers
    //@{
    void RegisterSourcePath( xml::xistream& xis, MIL_Config& config, std::map< std::string, tools::Path >& paths );
    void ReadModel( xml::xistream& xis, const std::map< std::string, tools::Path >& strSourcePaths,
                    const std::string& strEntityType, const T_MissionTypeNameMap& missionTypes,
                    const tools::Path& integrationDir );
    //@}

private:
    T_ModelMap                          automateModels_;
    T_ModelMap                          pionModels_;
    T_ModelMap                          populationModels_;
    T_ModelTypeMap                      modelTypes_;

    T_MissionTypeNameMap                unitMissionsNames_ ;
    T_MissionTypeNameMap                automatMissionsNames_ ;
    T_MissionTypeNameMap                populationMissionsNames_ ;
};

#include "DEC_Workspace.inl"

#endif // __DEC_Workspace_h_
