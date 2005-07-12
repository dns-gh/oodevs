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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_BooleanType.h $
// $Author: Age $
// $Modtime: 23/09/04 18:15 $
// $Revision: 2 $
// $Workfile: AGR_BooleanType.h $
//
// *****************************************************************************

#ifndef __AGR_BooleanType_h_
#define __AGR_BooleanType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_BooleanType
    @brief  AGR_BooleanType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_BooleanType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_BooleanType();
    virtual ~AGR_BooleanType();
    //@}

    //! @name Operations
    //@{
    virtual std::string MosInitialisationCode    ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_BooleanType( const AGR_BooleanType& );            //!< Copy constructor
    AGR_BooleanType& operator=( const AGR_BooleanType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_BooleanType_h_
