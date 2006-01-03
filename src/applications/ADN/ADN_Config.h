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

class ADN_ConfigDialog;

//*****************************************************************************
// Created: SBO 02-01-2006
//*****************************************************************************
class ADN_Config
{

public:
    //! @name Constructor/Destructor
    //@{
    explicit ADN_Config();
    virtual ~ADN_Config();
    //@}

    //! @name Operations
    //@{
    void Configure();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetSimPath     () const;
    const std::string& GetSimArguments() const;
    //@}
    
    //! @name Modifiers
    //@{
    void SetSimPath     ( const std::string& strPath );
    void SetSimArguments( const std::string& strArgs );
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
    std::string       strSimPath_;
    std::string       strSimArguments_;
};

#include "ADN_Config.inl"

#endif // __ADN_Config_h_