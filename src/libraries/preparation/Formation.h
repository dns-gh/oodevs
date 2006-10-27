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

#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class InstanciationComplete;
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
class Formation : public kernel::Formation_ABC
                , public kernel::Extension_ABC
                , public kernel::Updatable_ABC< kernel::InstanciationComplete >
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
    virtual unsigned long GetId() const;
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
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned long id_;
    QString name_;
    const kernel::HierarchyLevel_ABC* level_;
    //@}
};

#endif // __Formation_h_
