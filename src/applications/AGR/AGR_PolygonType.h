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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_PolygonType.h $
// $Author: Age $
// $Modtime: 24/09/04 15:21 $
// $Revision: 3 $
// $Workfile: AGR_PolygonType.h $
//
// *****************************************************************************

#ifndef __AGR_PolygonType_h_
#define __AGR_PolygonType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_PolygonType
    @brief  AGR_PolygonType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_PolygonType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_PolygonType();
    virtual ~AGR_PolygonType();
    //@}

    //! @name Operations
    //@{
    virtual std::string MosInitialisationCode    ( const AGR_Member& member ) const;
    virtual std::string Mos2InitialisationCode   ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_PolygonType( const AGR_PolygonType& );            //!< Copy constructor
    AGR_PolygonType& operator=( const AGR_PolygonType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_PolygonType_h_
