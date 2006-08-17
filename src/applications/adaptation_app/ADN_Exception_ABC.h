// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Exception_ABC.h $
// $Author: Ape $
// $Modtime: 21/03/05 18:35 $
// $Revision: 2 $
// $Workfile: ADN_Exception_ABC.h $
//
// *****************************************************************************

#ifndef __ADN_Exception_ABC_h_
#define __ADN_Exception_ABC_h_


// =============================================================================
/** @class  ADN_Exception_ABC
    @brief  ADN_Exception_ABC
*/
// Created: APE 2005-03-15
// =============================================================================
class ADN_Exception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_Exception_ABC() {}
    virtual ~ADN_Exception_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetExceptionTitle() const = 0;
    virtual std::string GetExceptionMessage() const = 0;
    //@}
};

#endif // __ADN_Exception_ABC_h_
