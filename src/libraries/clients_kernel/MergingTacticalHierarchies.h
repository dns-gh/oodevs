// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
    virtual void UpdateSymbolUpward();
    virtual void UpdateSymbolDownward();

    static std::string MaxLevel( const std::string& lhs, const std::string& rhs );
    static std::string IncreaseLevel( const std::string& level );
    static std::string DecreaseLevel( const std::string& level );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void RegisterSubordinate  (       kernel::Entity_ABC& entity );
    virtual void UnregisterSubordinate( const kernel::Entity_ABC& entity );
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    virtual void MergeSymbol( const kernel::Entity_ABC& entity );
    virtual void UpdateLevel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MergingTacticalHierarchies( const MergingTacticalHierarchies& );            //!< Copy constructor
    MergingTacticalHierarchies& operator=( const MergingTacticalHierarchies& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateSymbol();
    static char Level( const std::string& value );
    static unsigned Count( const std::string& value );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    Entity_ABC& entity_;
    bool symbolCanBeUpdated_;
    //@}
};

}

#endif // __MergingTacticalHierarchies_h_