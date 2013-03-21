//*****************************************************************************
//
// $Created: JDY 03-09-08 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Config.h $
// $Author: Ape $
// $Modtime: 10/02/05 16:10 $
// $Revision: 3 $
// $Workfile: ADN_Config.h $
//
//*****************************************************************************

#ifndef __ADN_Config_h_
#define __ADN_Config_h_

#include <tools/Path.h>

class ADN_ConfigDialog;

//*****************************************************************************
// Created: SBO 02-01-2006
//*****************************************************************************
class ADN_Config
{
public:
    //! @name Constructor/Destructor
    //@{
             ADN_Config();
    virtual ~ADN_Config();
    //@}

    //! @name Operations
    //@{
    void Configure();
    //@}

    //! @name Accessors
    //@{
    const tools::Path& GetSimPath     () const;
    const std::wstring& GetSimArguments() const;
    //@}

    //! @name Modifiers
    //@{
    void SetSimPath     ( const tools::Path& strPath );
    void SetSimArguments( const std::wstring& strArgs );
    //@}

private:
    //! @name Helpers
    //@{
    void Save();
    void Load();
    //@}

private:
    ADN_ConfigDialog* pDialog_;

    // config data
    tools::Path       strSimPath_;
    std::wstring      strSimArguments_;
};

#include "ADN_Config.inl"

#endif // __ADN_Config_h_