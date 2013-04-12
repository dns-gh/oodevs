// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatHierarchies_h_
#define __AutomatHierarchies_h_

#include "clients_gui/MergingTacticalHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"
#include "TacticalHierarchies.h"

// =============================================================================
/** @class  AutomatHierarchies
    @brief  AutomatHierarchies
*/
// Created: SBO 2006-10-26
// =============================================================================
class AutomatHierarchies : public gui::MergingTacticalHierarchies
                         , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~AutomatHierarchies();
    //@}

    //! @name Operations
    //@{
    void ChangeSuperior( kernel::Entity_ABC& superior );
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual std::string GetLevel() const;
    virtual void UpdateSymbolUpward();
    virtual void NotifySuperiorChanged( const kernel::Entity_ABC* newSuperior );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatHierarchies( const AutomatHierarchies& );            //!< Copy constructor
    AutomatHierarchies& operator=( const AutomatHierarchies& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    virtual void MergeSymbol( const kernel::Entity_ABC& entity );
    virtual void UpdateLevel();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC* superior_;
    std::string level_;
    //@}
};

#endif // __AutomatHierarchies_h_
