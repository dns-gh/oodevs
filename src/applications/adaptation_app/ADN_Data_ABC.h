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

#include <boost/noncopyable.hpp>
#include <list>
#include "ADN_Enums.h"

namespace xml
{
    class xistream;
    class xostream;
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
                   , private boost::noncopyable
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
    virtual void Reset() = 0;
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Initialize();
    virtual void Save();
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;
    virtual void ReadArchive( xml::xistream& input );
    virtual void WriteArchive( xml::xostream& output );
    virtual std::string GetInvalidDataErrorMsg() const;
    //@}

    //! @name Initializer
    //@{
    static void InitTranslations();
    //@}

protected:
    //! @name Member Data
    //@{
    static QString duplicateName_;
    static QString invalidDataOntab_;
    E_WorkspaceElements currentTab_;
    int subTab_;
    //@}
};

#endif // __ADN_Data_ABC_h_
