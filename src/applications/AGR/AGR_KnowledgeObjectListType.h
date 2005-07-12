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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_KnowledgeObjectListType.h $
// $Author: Age $
// $Modtime: 24/09/04 15:19 $
// $Revision: 5 $
// $Workfile: AGR_KnowledgeObjectListType.h $
//
// *****************************************************************************

#ifndef __AGR_KnowledgeObjectListType_h_
#define __AGR_KnowledgeObjectListType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_KnowledgeObjectListType
    @brief  AGR_KnowledgeObjectListType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_KnowledgeObjectListType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_KnowledgeObjectListType();
    virtual ~AGR_KnowledgeObjectListType();
    //@}

    //! @name Operations
    //@{
    virtual std::string SerializationCode        ( const AGR_Member& member ) const;
    virtual std::string ASNInitialisationCode    ( const AGR_Member& member ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_KnowledgeObjectListType( const AGR_KnowledgeObjectListType& );            //!< Copy constructor
    AGR_KnowledgeObjectListType& operator=( const AGR_KnowledgeObjectListType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_KnowledgeObjectListType_h_
