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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_KnowledgeAgentType.h $
// $Author: Age $
// $Modtime: 23/09/04 18:23 $
// $Revision: 5 $
// $Workfile: AGR_KnowledgeAgentType.h $
//
// *****************************************************************************

#ifndef __AGR_KnowledgeAgentType_h_
#define __AGR_KnowledgeAgentType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_KnowledgeAgentType
    @brief  AGR_KnowledgeAgentType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_KnowledgeAgentType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_KnowledgeAgentType();
    virtual ~AGR_KnowledgeAgentType();
    //@}

    //! @name Operations
    //@{
    virtual std::string ASNInitialisationCode    ( const AGR_Member& member ) const;
    virtual std::string SerializationCode        ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_KnowledgeAgentType( const AGR_KnowledgeAgentType& );            //!< Copy constructor
    AGR_KnowledgeAgentType& operator=( const AGR_KnowledgeAgentType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_KnowledgeAgentType_h_
