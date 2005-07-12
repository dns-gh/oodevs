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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_AgentType.h $
// $Author: Age $
// $Modtime: 23/09/04 18:15 $
// $Revision: 2 $
// $Workfile: AGR_AgentType.h $
//
// *****************************************************************************

#ifndef __AGR_AgentType_h_
#define __AGR_AgentType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_AgentType
    @brief  AGR_AgentType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_AgentType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_AgentType();
    virtual ~AGR_AgentType();
    //@}

    //! @name Operations
    //@{
    virtual std::string ASNInitialisationCode    ( const AGR_Member& member ) const;
    //@}


private:
    //! @name Copy/Assignement
    //@{
    AGR_AgentType( const AGR_AgentType& );            //!< Copy constructor
    AGR_AgentType& operator=( const AGR_AgentType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_AgentType_h_
