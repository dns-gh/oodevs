// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class FormationLevels;
}

namespace xml
{
    class xistream;
}

class IdManager;

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: SBO 2006-09-19
// =============================================================================
class Formation : public kernel::EntityImplementation< kernel::Formation_ABC >
                , public kernel::Extension_ABC
                , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Formation( kernel::Controller& controller, const kernel::HierarchyLevel_ABC& level, IdManager& idManager );
             Formation( xml::xistream& xis, kernel::Controller& controller, const kernel::FormationLevels& levels, IdManager& idManager );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
    void Rename( const QString& name );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::HierarchyLevel_ABC* level_;
    //@}
};

#endif // __Formation_h_
