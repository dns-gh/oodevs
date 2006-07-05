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

#include "Positions.h"
class PopulationKnowledge;

// =============================================================================
/** @class  PopulationKnowledgePositions
    @brief  PopulationKnowledgePositions
*/
// Created: AGE 2006-05-18
// =============================================================================
class PopulationKnowledgePositions : public Positions
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationKnowledgePositions( const PopulationKnowledge& k );
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

private:
    //! @name Member data
    //@{
    const PopulationKnowledge& knowledge_;
    //@}
};

#endif // __PopulationKnowledgePositions_h_
