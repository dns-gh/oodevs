// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledgePositions_h_
#define __PopulationKnowledgePositions_h_

#include "clients_kernel/Positions.h"

class PopulationKnowledge_ABC;

// =============================================================================
/** @class  PopulationKnowledgePositions
    @brief  PopulationKnowledgePositions
*/
// Created: AGE 2006-05-18
// =============================================================================
class PopulationKnowledgePositions : public kernel::Positions
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationKnowledgePositions( const PopulationKnowledge_ABC& k );
    virtual ~PopulationKnowledgePositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationKnowledgePositions( const PopulationKnowledgePositions& );            //!< Copy constructor
    PopulationKnowledgePositions& operator=( const PopulationKnowledgePositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    const kernel::Positions& GetRealPosition() const;
    //@}

private:
    //! @name Member data
    //@{
    const PopulationKnowledge_ABC& knowledge_;
    //@}
};

#endif // __PopulationKnowledgePositions_h_
