// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgePositions_h_
#define __ObjectKnowledgePositions_h_

#include "LocationPositions.h"
#include "Updatable_ABC.h"

// =============================================================================
/** @class  ObjectKnowledgePositions
    @brief  ObjectKnowledgePositions
*/
// Created: AGE 2006-05-18
// =============================================================================
class ObjectKnowledgePositions : public LocationPositions
                               , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgePositions( const CoordinateConverter_ABC& converter );
    virtual ~ObjectKnowledgePositions();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledgePositions( const ObjectKnowledgePositions& );            //!< Copy constructor
    ObjectKnowledgePositions& operator=( const ObjectKnowledgePositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message );
    //@}
};

#endif // __ObjectKnowledgePositions_h_
