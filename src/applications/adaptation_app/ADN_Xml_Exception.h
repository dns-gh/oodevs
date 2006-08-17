// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-07-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Xml_Exception.h $
// $Author: Ape $
// $Modtime: 21/03/05 18:35 $
// $Revision: 5 $
// $Workfile: ADN_Xml_Exception.h $
//
// *****************************************************************************

#ifndef __ADN_Xml_Exception_h_
#define __ADN_Xml_Exception_h_

#include "ADN_Exception_ABC.h"


// =============================================================================
/** @class  ADN_Xml_Exception
    @brief  ADN_Xml_Exception
*/
// Created: AGN 2004-07-01
// =============================================================================
class ADN_Xml_Exception
: public ADN_Exception_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Xml_Exception( const std::string& strFile, const std::string& strContext, const std::string& strMessage );
     ADN_Xml_Exception( const std::string& strContext, const std::string& strMessage );
     ADN_Xml_Exception( const ADN_Xml_Exception& );            //!< Copy constructor
    ~ADN_Xml_Exception();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetExceptionTitle() const;
    virtual std::string GetExceptionMessage() const;
    //@}

    //! @name Accessors
    //@{
    const std::string& GetFileName() const;
    const std::string& GetContext() const;
    const std::string& GetErrorMessage() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string strFile_;
    std::string strContext_;
    std::string strMessage_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ADN_Xml_Exception::GetFileName
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
inline
const std::string& ADN_Xml_Exception::GetFileName() const
{
    return strFile_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Xml_Exception::GetContext
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
inline
const std::string& ADN_Xml_Exception::GetContext() const
{
    return strContext_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Xml_Exception::GetMessage
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
inline
const std::string& ADN_Xml_Exception::GetErrorMessage() const
{
    return strMessage_;
}


#endif // __ADN_Xml_Exception_h_
