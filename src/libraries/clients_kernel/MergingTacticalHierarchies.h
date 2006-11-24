// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MergingTacticalHierarchies_h_
#define __MergingTacticalHierarchies_h_

#include "TacticalHierarchies.h"
#include "EntityHierarchies.h"

namespace kernel
{

// =============================================================================
/** @class  MergingTacticalHierarchies
    @brief  MergingTacticalHierarchies
*/
// Created: AGE 2006-11-22
// =============================================================================
class MergingTacticalHierarchies : public EntityHierarchies< TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             MergingTacticalHierarchies( Controller& controller, Entity_ABC& entity, Entity_ABC* superior );
    virtual ~MergingTacticalHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetSymbol() const;
    virtual void UpdateSymbol( bool up = true );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void RegisterSubordinate  (       kernel::Entity_ABC& entity );
    virtual void UnregisterSubordinate( const kernel::Entity_ABC& entity );
    virtual void MergeSymbol( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MergingTacticalHierarchies( const MergingTacticalHierarchies& );            //!< Copy constructor
    MergingTacticalHierarchies& operator=( const MergingTacticalHierarchies& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    std::string symbol_;
    //@}
};

}

#endif // __MergingTacticalHierarchies_h_
