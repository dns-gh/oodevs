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
        void WriteArchive( xml::xostream& );

    public:
        ADN_Type_String szDecisional_;
        ADN_Type_String szSizes_;
        ADN_Type_String szUrban_;
        ADN_Type_String szArmors_;
        ADN_Type_String szActiveProtections_;
        ADN_Type_String szDotationNatures_;
        ADN_Type_String szObjects_;
        ADN_Type_String szEquipements_;
        ADN_Type_String szLaunchers_;
        ADN_Type_String szWeapons_;
        ADN_Type_String szSensors_;
        ADN_Type_String szComponents_;
        ADN_Type_String szUnits_;
        ADN_Type_String szAutomata_;
        ADN_Type_String szNBC_;
        ADN_Type_String szHealth_;
        ADN_Type_String szHumanFactors_;
        ADN_Type_String szBreakdowns_;
        ADN_Type_String szKnowledgeGroups_;
        ADN_Type_String szMaintenance_;
        ADN_Type_String szSupply_;
        ADN_Type_String szCom_;
        ADN_Type_String szPopulation_;
        ADN_Type_String szPeople_;
        ADN_Type_String szReports_;
        ADN_Type_String szModels_;
        ADN_Type_String szMissions_;
        ADN_Type_String szResourceNetworks_;
        ADN_Type_String szFireClasses_;
        // $$$$ NLD 2007-01-15: files not loaded!
        ADN_Type_String szPathfinder_;
        ADN_Type_String szObjectNames_;
        ADN_Type_String szHumanProtections_;
        ADN_Type_String szMedicalTreatment_;
        ADN_Type_String szExtensions_;
        ADN_Type_String szDrawingTemplates_;
        ADN_Type_String szScores_;
        ADN_Type_String szSymbols_;
        ADN_Type_String szFilters_;
        ADN_Type_String szUrbanTemplates_;
    };

//*****************************************************************************
    class FileInfos : private boost::noncopyable
    {
    public:
                 FileInfos();
        explicit FileInfos( const std::string& filename );

        // return filename in working directory or tmp directory ref
        ADN_Type_String&       GetFileName();
        const std::string      GetFileNameFull();

        FileInfos& operator =(const std::string&filename);

    public:
        static std::string     szUntitled_;

    private:
        ADN_Type_String        szFileName_;
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

        void               SetWorkingDirectory( const std::string& filename );
        ADN_Type_String&   GetWorkingDirectory();  //!< Returns the working directory

        void               UseTempDirectory( bool bUse );
        ADN_Type_String&   GetTempDirectory();     //!< Returns a temporary directory

        const std::string  GetSaveDirectory(); //!< Returns either the working or the temporary directory according to the

        std::string        GetRelativePath(const std::string& full, E_WorkDir e=eWorking); //!< Given "the/whole/path/toto/toto.xml" returns "/toto/toto.xml"

        // private
        std::string        GetFullPath(const std::string& part, E_WorkDir e=eWorking); //!< Given "/toto/toto.xml" returns "the/whole/path/toto/toto.xml"

    private:
        ADN_Type_String     szWorkingDir_;
        ADN_Type_String     szTempDir_;
        bool                bTmpActivated_;
    };


//*****************************************************************************
public:
             ADN_Project_Data();
    virtual ~ADN_Project_Data();

    void SetFile( const std::string& strFile );

    virtual void FilesNeeded( T_StringList& vFiles ) const;
    virtual void Reset();
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Save();

    DataInfos&           GetDataInfos();
    const DataInfos&     GetDataInfos() const;
    FileInfos&           GetFileInfos();
    static WorkDirInfos& GetWorkDirInfos();

private:
    void FilterNode( const std::string& node, xml::xistream& xis, xml::xostream& xos );

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
ADN_Type_String& ADN_Project_Data::WorkDirInfos::GetWorkingDirectory()
{
    return szWorkingDir_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::WorkDirInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
inline
ADN_Type_String& ADN_Project_Data::WorkDirInfos::GetTempDirectory()
{
    return szTempDir_;
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
inline
const std::string ADN_Project_Data::WorkDirInfos::GetSaveDirectory()
{
    return bTmpActivated_ ? szTempDir_.GetData() : szWorkingDir_.GetData();
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::FileInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
inline
ADN_Type_String& ADN_Project_Data::FileInfos::GetFileName()
{
    return szFileName_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::GetFileNameFull
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
inline
const std::string ADN_Project_Data::FileInfos::GetFileNameFull()
{
    return workDir_.GetFullPath( szFileName_.GetData(), ADN_Project_Data::WorkDirInfos::eWorking );
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