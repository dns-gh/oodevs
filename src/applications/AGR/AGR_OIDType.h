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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OIDType.h $
// $Author: Age $
// $Modtime: 23/09/04 18:29 $
// $Revision: 3 $
// $Workfile: AGR_OIDType.h $
//
// *****************************************************************************

#ifndef __AGR_OIDType_h_
#define __AGR_OIDType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_OIDType
    @brief  AGR_OIDType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_OIDType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_OIDType();
    virtual ~AGR_OIDType();
    //@}

    //! @name Operations
    //@{
    virtual std::string Mos2InitialisationCode         ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_OIDType( const AGR_OIDType& );            //!< Copy constructor
    AGR_OIDType& operator=( const AGR_OIDType& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_OIDType_h_
