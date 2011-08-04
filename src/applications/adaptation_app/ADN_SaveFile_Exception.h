// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-12-09 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_SaveFile_Exception.h $
// $Author: Ape $
// $Modtime: 21/03/05 18:35 $
// $Revision: 2 $
// $Workfile: ADN_SaveFile_Exception.h $
//
// *****************************************************************************

#ifndef __ADN_SaveFile_Exception_h_
#define __ADN_SaveFile_Exception_h_

#include "ADN_Exception_ABC.h"

// =============================================================================
/** @class  ADN_SaveFile_Exception
@brief  ADN_SaveFile_Exception
*/
// Created: AGN 2003-12-09
// =============================================================================
class ADN_SaveFile_Exception
    : public ADN_Exception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_SaveFile_Exception( const std::string& strFileName );
    ADN_SaveFile_Exception( const ADN_SaveFile_Exception& ); //!< Copy constructor
    virtual ~ADN_SaveFile_Exception();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetExceptionTitle() const;
    virtual std::string GetExceptionMessage() const;
    //@}

    //! @name Accessors
    //@{
    const std::string& GetFileName() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string strFileName_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ADN_SaveFile_Exception::GetFileName
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
inline
const std::string& ADN_SaveFile_Exception::GetFileName() const
{
    return strFileName_;
}


#endif // __ADN_SaveFile_Exception_h_
