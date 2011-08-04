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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_DataException.h $
// $Author: Ape $
// $Modtime: 21/03/05 18:35 $
// $Revision: 4 $
// $Workfile: ADN_DataException.h $
//
// *****************************************************************************

#ifndef __ADN_DataException_h_
#define __ADN_DataException_h_

#include "ADN_Exception_ABC.h"

// =============================================================================
/** @class  ADN_DataException
    @brief  ADN_DataException
*/
// Created: AGN 2003-12-09
// =============================================================================
class ADN_DataException
: public ADN_Exception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     ADN_DataException( const std::string& strErrorTitle,
                        const std::string& strErrorDescription,
                        const std::string& strErrorSolution = std::string() );
     ADN_DataException( const ADN_DataException& );            //!< Copy constructor
    virtual ~ADN_DataException();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetExceptionTitle() const;
    virtual std::string GetExceptionMessage() const;
    //@}


    //! @name Accessors
    //@{
    const std::string& GetTitle();
    const std::string& GetDescription();
    const std::string& GetSolution();
    //@}

private:
    //! @name Member data
    //@{
    std::string strErrorTitle_;
    std::string strErrorDescription_;
    std::string strErrorSolution_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ADN_DataException::GetTitle
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
inline
const std::string& ADN_DataException::GetTitle()
{
    return strErrorTitle_;
}


// -----------------------------------------------------------------------------
// Name: ADN_DataException::GetDescription
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
inline
const std::string& ADN_DataException::GetDescription()
{
    return strErrorDescription_;
}


// -----------------------------------------------------------------------------
// Name: ADN_DataException::GetSolution
// Created: AGN 2003-12-09
// -----------------------------------------------------------------------------
inline
const std::string& ADN_DataException::GetSolution()
{
    return strErrorSolution_;
}


#endif // __ADN_DataException_h_
