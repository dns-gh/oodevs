//*****************************************************************************
// 
// $Created: AGN 03-06-16 $
// $Archive: /MVW_v10/Build/SDK/SSC/src/SSC_Workspace.h $
// $Author: Age $
// $Modtime: 22/03/05 15:06 $
// $Revision: 8 $
// $Workfile: SSC_Workspace.h $
// 
//*****************************************************************************

#ifndef __SSC_Workspace_h_
#define __SSC_Workspace_h_

#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_Archive/MT_Archive_lib.h"

#include "DIA/DIA_Kernel_lib.h"

//*****************************************************************************
// Created: AGN 03-06-16
//*****************************************************************************
class SSC_Workspace
{
    MT_COPYNOTALLOWED( SSC_Workspace )

public:
     SSC_Workspace( int argc, char** argv  );
    ~SSC_Workspace();

private:
    void ParseArguments( int argc, char** argv );

    std::string GetDecFile         () const;
    bool        CheckFilesDepencies( const std::string& strArchiveFile );
    bool        NeedScriptParsing  ( bool bNeedParsing, const std::string& strArchiveName, const std::string& strFileName, const std::string& strOpenedFileArchiveName );
    bool        FindModelFromScript( const std::string strFileName ) const;
    void        Initialize         ( MT_InputArchive_ABC& decArchive );
    void        InitializeModels   ( const std::string& strFile, bool bNeedScriptParsing, const std::string& strArchivePath, const std::string& strSourcePath );
    std::string InitializeModel    ( const std::string& strName, MT_InputArchive_ABC& input, bool bNeedParsing, const std::string& strArchivePath, const std::string& strSourcePath );
    bool        FileChanged( const std::string& strFileName, time_t since );

private:
    //! @name Types
    //@{
    typedef std::map< std::string /*name*/, std::string /*scriptname*/ >   T_ModelMap;
    typedef T_ModelMap::const_iterator                                   CIT_ModelMap;

    typedef std::map< std::string, time_t > T_ChangeMap;
    typedef T_ChangeMap::iterator          IT_ChangeMap;
    //@}

private:
    bool        bGeneratesPreprocessedFiles_;
    T_ModelMap  models_;
    T_ChangeMap changedFiles_;
};


#endif // __SSC_Workspace_h_