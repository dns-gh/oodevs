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
#include "astec_kernel/Updatable_ABC.h"

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

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
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

private:
    //! @name Member data
    //@{
    bool realObjectKnown_;
    bool perceived_;
    //@}
};

#endif // __ObjectKnowledgePositions_h_
