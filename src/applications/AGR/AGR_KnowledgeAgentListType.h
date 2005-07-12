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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_KnowledgeAgentListType.h $
// $Author: Age $
// $Modtime: 24/09/04 15:18 $
// $Revision: 5 $
// $Workfile: AGR_KnowledgeAgentListType.h $
//
// *****************************************************************************

#ifndef __AGR_KnowledgeAgentListType_h_
#define __AGR_KnowledgeAgentListType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_KnowledgeAgentListType
    @brief  AGR_KnowledgeAgentListType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_KnowledgeAgentListType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_KnowledgeAgentListType();
    virtual ~AGR_KnowledgeAgentListType();
    //@}

    //! @name Operations
    //@{
    virtual std::string SerializationCode        ( const AGR_Member& member ) const;
    virtual std::string ASNInitialisationCode    ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_KnowledgeAgentListType( const AGR_KnowledgeAgentListType& );            //!< Copy constructor
    AGR_KnowledgeAgentListType& operator=( const AGR_KnowledgeAgentListType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_KnowledgeAgentListType_h_
