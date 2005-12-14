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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_KnowledgePopulationType.h $
// $Author: Age $
// $Modtime: 23/09/04 18:24 $
// $Revision: 5 $
// $Workfile: AGR_KnowledgePopulationType.h $
//
// *****************************************************************************

#ifndef __AGR_KnowledgePopulationType_h_
#define __AGR_KnowledgePopulationType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_KnowledgePopulationType
    @brief  AGR_KnowledgePopulationType
*/
// Created: AGE 2004-09-13
// =============================================================================
class AGR_KnowledgePopulationType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_KnowledgePopulationType();
    virtual ~AGR_KnowledgePopulationType();
    //@}

    //! @name Operations
    //@{
    virtual std::string ASNInitialisationCode    ( const AGR_Member& member ) const;
    virtual std::string SerializationCode        ( const AGR_Member& member ) const;
    virtual std::string MOS2RCCode               ( const std::string& strAsnParam ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_KnowledgePopulationType( const AGR_KnowledgePopulationType& );            //!< Copy constructor
    AGR_KnowledgePopulationType& operator=( const AGR_KnowledgePopulationType& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_KnowledgePopulationType_h_
