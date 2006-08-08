// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-13 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_DotationDType.h $
// $Author: Age $
// $Modtime: 24/09/04 15:20 $
// $Revision: 3 $
// $Workfile: AGR_DotationDType.h $
//
// *****************************************************************************

#ifndef __AGR_DotationDType_h_
#define __AGR_DotationDType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_DotationDType
    @brief  AGR_DotationDType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_DotationDType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_DotationDType();
    virtual ~AGR_DotationDType();
    //@}

    //! @name Operations
    //@{
    virtual std::string Mos2InitialisationCode   ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_DotationDType( const AGR_DotationDType& );            //!< Copy constructor
    AGR_DotationDType& operator=( const AGR_DotationDType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_DotationDType_h_
