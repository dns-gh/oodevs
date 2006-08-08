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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_PointType.h $
// $Author: Age $
// $Modtime: 24/09/04 15:20 $
// $Revision: 3 $
// $Workfile: AGR_PointType.h $
//
// *****************************************************************************

#ifndef __AGR_PointType_h_
#define __AGR_PointType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_PointType
    @brief  AGR_PointType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_PointType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_PointType();
    virtual ~AGR_PointType();
    //@}

    //! @name Operations
    //@{
    virtual std::string ASNInitialisationCode    ( const AGR_Member& member ) const;
    virtual bool AllowOptionalMembers() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_PointType( const AGR_PointType& );            //!< Copy constructor
    AGR_PointType& operator=( const AGR_PointType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_PointType_h_
