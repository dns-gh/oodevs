//*****************************************************************************
//
// $Created: JDY 03-06-23 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Project_Data.h $
// $Author: Ape $
// $Modtime: 14/04/05 10:57 $
// $Revision: 8 $
// $Workfile: ADN_Project_Data.h $
//
//*****************************************************************************

#ifndef __ADN_Project_Data_h_
#define __ADN_Project_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include <map>
#include <boost/noncopyable.hpp>

namespace tools
{
    class Loader_ABC;
}

namespace xml { class xistream; }

//*****************************************************************************
// Created: JDY 03-06-20
//*****************************************************************************
class ADN_Project_Data : public ADN_Data_ABC
{

public:
    class DataInfos : private boost::noncopyable
    {

    public:
        DataInfos();

        void ReadArchive( xml::xistream& );
        void WriteArchive( xml::xostream& ) const;

        bool IsReadOnly();
        void DisableReadOnly();
        void SetNoReadOnly();

    public:
        tools::Path szDecisional_;
        tools::Path szSizes_;
        tools::Path szUrban_;
        tools::Path szArmors_;
        tools::Path szActiveProtections_;
        tools::Path szDotationNatures_;
        tools::Path szLogisticSupplyClasses_;
        tools::Path szObjects_;
        tools::Path szEquipements_;
        tools::Path szLaunchers_;
        tools::Path szWeapons_;
        tools::Path szSensors_;
        tools::Path szComponents_;
        tools::Path szUnits_;
        tools::Path szAutomata_;
        tools::Path szNBC_;
        tools::Path szHealth_;
        tools::Path szHumanFactors_;
        tools::Path szBreakdowns_;
        tools::Path szKnowledgeGroups_;
        tools::Path szMaintenance_;
        tools::Path szSupply_;
        tools::Path szCom_;
        tools::Path szPopulation_;
        tools::Path szPeople_;
        tools::Path szReports_;
        tools::Path szModels_;
        tools::Path szMissions_;
        tools::Path szResourceNetworks_;
        tools::Path szFireClasses_;
        tools::Path szFuneral_;
        tools::Path szUrbanTemplates_;
        tools::Path szDisasters_;
        tools::Path szStages_;
        tools::Path szLanguages_;
        tools::Path szLocalesDirectory_;
        // $$$$ NLD 2007-01-15: files not loaded!
        tools::Path szPathfinder_;
        tools::Path szObjectNames_;
        tools::Path szExtensions_;
        tools::Path szDrawingTemplates_;
        tools::Path szScores_;
        tools::Path szSymbols_;
        tools::Path szFilters_;
        // $$$$ NPT 2012-07-27: Mission sheets data path
        tools::Path szUnitsMissionPath_;
        tools::Path szAutomataMissionPath_;
        tools::Path szCrowdsMissionPath_;
        tools::Path szFragOrdersMissionPath_;
        // $$$$ ABR 2013-01-21: Symbols directory
        tools::Path szMissionSheetXslPath_;
        tools::Path szSymbolsPath_;

    private:
        bool readOnlyEnabled_;
        bool readOnly_;
    };

//*****************************************************************************
    class FileInfos : private boost::noncopyable
    {
    public:
                 FileInfos();
        explicit FileInfos( const tools::Path& filename );

        // return filename in working directory or tmp directory ref
        const tools::Path& GetFileName() const;
        tools::Path GetFileNameFull() const;

        FileInfos& operator =( const tools::Path& filename );

    public:
        static tools::Path szUntitled_;

    private:
        tools::Path szFileName_;
    };

//*****************************************************************************
    class WorkDirInfos : private boost::noncopyable
    {
    public:
        enum E_WorkDir
        {
            eWorking,
            eTemp
        };

    public:
        WorkDirInfos();

        void SetWorkingDirectory( const tools::Path& filename );
        ADN_Type_Path& GetWorkingDirectory();                         //!< Returns the working directory
        ADN_Type_Path& GetOldWorkingDirectory();                      //!< Returns the old working directory

        void UseTempDirectory( bool bUse );
        const tools::Path& GetTempDirectory() const;                            //!< Returns a temporary directory
        tools::Path GetSaveDirectory() const;                                   //!< Returns either the working or the temporary directory according to the current mode

        tools::Path GetRelativePath(const tools::Path& full, E_WorkDir e = eWorking ) const; //!< Given "the/whole/path/toto/toto.xml" returns "/toto/toto.xml"
        tools::Path GetFullPath(const tools::Path& part, E_WorkDir e = eWorking ) const; //!< Given "/toto/toto.xml" returns "the/whole/path/toto/toto.xml"

    private:
        ADN_Type_Path szWorkingDir_;
        ADN_Type_Path szOldWorkingDir_;
        tools::Path   szTempDir_;
        bool          bTmpActivated_;
    };

//*****************************************************************************
public:
             ADN_Project_Data();
    virtual ~ADN_Project_Data();

    void SetFile( const tools::Path& strFile );

    void New( const tools::Path& filename );
    virtual void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Save( const tools::Loader_ABC& fileLoader );

    DataInfos&           GetDataInfos();
    const DataInfos&     GetDataInfos() const;
    FileInfos&           GetFileInfos();
    tools::Path          GetMissionDir( E_MissionType missionType ) const;
    static WorkDirInfos& GetWorkDirInfos();
    tools::Path          GetLocalDir() const;
    tools::Path          GetLocalMissionDir( E_MissionType missionType ) const;

private:
    void FilterNode( const std::string& node, xml::xistream& xis, xml::xostream& xos );
    void WritePathfind( xml::xistream& xis, const tools::Path& path );

public:
    DataInfos dataInfos_;
    FileInfos szFile_;
    std::map< std::string, std::string > addedObjects_;
    static WorkDirInfos workDir_;
};

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::WorkDirInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
inline
ADN_Type_Path& ADN_Project_Data::WorkDirInfos::GetWorkingDirectory()
{
    return szWorkingDir_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::GetOldWorkingDirectory
// Created: SLI 2014-10-09
// -----------------------------------------------------------------------------
inline
ADN_Type_Path& ADN_Project_Data::WorkDirInfos::GetOldWorkingDirectory()
{
    return szOldWorkingDir_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::WorkDirInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
inline
const tools::Path& ADN_Project_Data::WorkDirInfos::GetTempDirectory() const
{
    return szTempDir_;
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
inline
tools::Path ADN_Project_Data::WorkDirInfos::GetSaveDirectory() const
{
    return bTmpActivated_ ? szTempDir_ : szWorkingDir_.GetData();
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::FileInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
inline
const tools::Path& ADN_Project_Data::FileInfos::GetFileName() const
{
    return szFileName_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::GetFileNameFull
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
inline
tools::Path ADN_Project_Data::FileInfos::GetFileNameFull() const
{
    return workDir_.GetFullPath( szFileName_, ADN_Project_Data::WorkDirInfos::eWorking );
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::GetDataInfos
// Created: APE 2004-11-09
// -----------------------------------------------------------------------------
inline
ADN_Project_Data::DataInfos& ADN_Project_Data::GetDataInfos()
{
    return dataInfos_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::GetDataInfos
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
inline
const ADN_Project_Data::DataInfos& ADN_Project_Data::GetDataInfos() const
{
    return dataInfos_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_String
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
inline
ADN_Project_Data::FileInfos& ADN_Project_Data::GetFileInfos()
{
    return szFile_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::WorkDirInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
inline
ADN_Project_Data::WorkDirInfos& ADN_Project_Data::GetWorkDirInfos()
{
    return workDir_;
}

#endif // __ADN_Project_Data_h_
