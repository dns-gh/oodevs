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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_IntegerType.h $
// $Author: Nld $
// $Modtime: 14/03/05 17:39 $
// $Revision: 4 $
// $Workfile: AGR_IntegerType.h $
//
// *****************************************************************************

#ifndef __AGR_IntegerType_h_
#define __AGR_IntegerType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_IntegerType
    @brief  AGR_IntegerType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_IntegerType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_IntegerType();
             AGR_IntegerType( const std::string& strAsnType, unsigned int nMin = 0, int nMax = std::numeric_limits< unsigned int >::max() );
    virtual ~AGR_IntegerType();
    //@}

    //! @name Operations
    //@{
    virtual std::string TesterSerializationCode  ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_IntegerType( const AGR_IntegerType& );            //!< Copy constructor
    AGR_IntegerType& operator=( const AGR_IntegerType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nMin_;
    unsigned int nMax_;
    //@}
};

#endif // __AGR_IntegerType_h_
