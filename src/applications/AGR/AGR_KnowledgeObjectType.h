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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_KnowledgeObjectType.h $
// $Author: Age $
// $Modtime: 23/09/04 18:24 $
// $Revision: 5 $
// $Workfile: AGR_KnowledgeObjectType.h $
//
// *****************************************************************************

#ifndef __AGR_KnowledgeObjectType_h_
#define __AGR_KnowledgeObjectType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_KnowledgeObjectType
    @brief  AGR_KnowledgeObjectType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_KnowledgeObjectType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_KnowledgeObjectType();
    virtual ~AGR_KnowledgeObjectType();
    //@}

    //! @name Operations
    //@{
    virtual std::string ASNInitialisationCode    ( const AGR_Member& member ) const;
    virtual std::string SerializationCode        ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_KnowledgeObjectType( const AGR_KnowledgeObjectType& );            //!< Copy constructor
    AGR_KnowledgeObjectType& operator=( const AGR_KnowledgeObjectType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_KnowledgeObjectType_h_
