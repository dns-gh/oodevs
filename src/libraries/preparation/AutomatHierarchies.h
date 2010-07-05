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

#include "clients_kernel/MergingTacticalHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"

// =============================================================================
/** @class  AutomatHierarchies
    @brief  AutomatHierarchies
*/
// Created: SBO 2006-10-26
// =============================================================================
class AutomatHierarchies : public kernel::MergingTacticalHierarchies
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
    virtual void UpdateSymbol( bool up = true );
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
    static std::string Max( const std::string& lhs, const std::string& rhs );
    static std::string Increase( const std::string& value );
    static char Level( const std::string& value );
    static unsigned Count( const std::string& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC* superior_;
    std::string level_;
    //@}
};

#endif // __AutomatHierarchies_h_
