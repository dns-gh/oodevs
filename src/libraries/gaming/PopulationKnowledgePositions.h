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

namespace kernel
{
    class PopulationKnowledge_ABC;
}

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
    explicit PopulationKnowledgePositions( const kernel::PopulationKnowledge_ABC& k );
    virtual ~PopulationKnowledgePositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledgePositions( const PopulationKnowledgePositions& );            //!< Copy constructor
    PopulationKnowledgePositions& operator=( const PopulationKnowledgePositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    const kernel::Positions& GetRealPosition() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::PopulationKnowledge_ABC& knowledge_;
    //@}
};

#endif // __PopulationKnowledgePositions_h_
