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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_GDHType.h $
// $Author: Age $
// $Modtime: 23/09/04 18:20 $
// $Revision: 2 $
// $Workfile: AGR_GDHType.h $
//
// *****************************************************************************

#ifndef __AGR_GDHType_h_
#define __AGR_GDHType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_GDHType
    @brief  AGR_GDHType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_GDHType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_GDHType();
    virtual ~AGR_GDHType();
    //@}

    //! @name Operations
    //@{
    virtual std::string ASNInitialisationCode    ( const AGR_Member& member ) const;
    virtual std::string MosInitialisationCode    ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_GDHType( const AGR_GDHType& );            //!< Copy constructor
    AGR_GDHType& operator=( const AGR_GDHType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_GDHType_h_
