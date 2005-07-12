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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_DoubleType.h $
// $Author: Age $
// $Modtime: 23/09/04 18:16 $
// $Revision: 3 $
// $Workfile: AGR_DoubleType.h $
//
// *****************************************************************************

#ifndef __AGR_DoubleType_h_
#define __AGR_DoubleType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_DoubleType
    @brief  AGR_DoubleType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_DoubleType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_DoubleType();
    virtual ~AGR_DoubleType();
    //@}

    //! @name Operations
    //@{
    virtual std::string MosInitialisationCode    ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_DoubleType( const AGR_DoubleType& );            //!< Copy constructor
    AGR_DoubleType& operator=( const AGR_DoubleType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_DoubleType_h_
