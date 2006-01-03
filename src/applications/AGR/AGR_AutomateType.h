// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-10-05 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_AutomateType.h $
// $Author: Age $
// $Modtime: 5/10/04 9:50 $
// $Revision: 1 $
// $Workfile: AGR_AutomateType.h $
//
// *****************************************************************************

#ifndef __AGR_AutomateType_h_
#define __AGR_AutomateType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_AutomateType
    @brief  AGR_AutomateType
*/
// Created: AGE 2004-10-05
// =============================================================================
class AGR_AutomateType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_AutomateType();
    virtual ~AGR_AutomateType();
    //@}

    //! @name Operations
    //@{
    virtual std::string ASNInitialisationCode    ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_AutomateType( const AGR_AutomateType& );            //!< Copy constructor
    AGR_AutomateType& operator=( const AGR_AutomateType& ); //!< Assignement operator
    //@}
};

#endif // __AGR_AutomateType_h_
