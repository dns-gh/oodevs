// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Data_ABC_h_
#define __ADN_Data_ABC_h_

#include "ADN_Enums.h"
#include "ADN_XmlStreamOperators.h"
#include "ADN_Tools.h"
#include "ADN_WorkspaceElement.h"

namespace kernel
{
    class Context;
    class XmlTranslations;
}

namespace tools
{
    class Loader_ABC;
}

class ADN_ConsistencyChecker;

// =============================================================================
/** @class  ADN_Data_ABC
    @brief  ADN_Data_ABC
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Data_ABC : public QObject
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Data_ABC( E_WorkspaceElements currentTab, int subTab = -1 );
    virtual ~ADN_Data_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( tools::Path::T_Paths& vFiles ) const = 0;
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Initialize();
    virtual void Save();
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;
    virtual void ReadArchive( xml::xistream& input );
    virtual void WriteArchive( xml::xostream& output );
    virtual void LoadTranslations( const tools::Path& xmlFile, kernel::XmlTranslations* translations = 0 );
    virtual std::string GetInvalidDataErrorMsg() const;
    boost::shared_ptr< kernel::Context > GetContext( const std::string& context ) const;
    //@}

    //! @name Initializer
    //@{
    static void InitQtTranslations();
    //@}

protected:
    //! @name Types
    //@{
    typedef boost::function< void ( xml::xistream& ) > T_Loader;
    typedef boost::function< void ( xml::xostream& ) > T_Saver;
    //@}

    //! @name Helpers
    //@{
    virtual void LoadFile( const tools::Loader_ABC& fileLoader, const tools::Path& xmlFile, T_Loader loader );
    virtual void SaveFile( const tools::Path& xmlFile, T_Saver saver );
    virtual void SaveTranslations( const tools::Path& xmlFile, kernel::XmlTranslations* translations = 0 );
    //@}

protected:
    //! @name Member Data
    //@{
    static QString duplicateName_;
    static QString invalidDataOntab_;
    E_WorkspaceElements currentTab_;
    std::auto_ptr< kernel::XmlTranslations > translations_;
    int subTab_;
    //@}
};

#endif // __ADN_Data_ABC_h_
