// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dotations_h_
#define __Dotations_h_

#include "HierarchicExtension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Dotations_ABC.h"
#include <tools/Resolver.h>

namespace sword
{
    class UnitAttributes;
}

namespace kernel
{
    class Controller;
    class DotationType;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

class Dotation;

// =============================================================================
/** @class  Dotations
    @brief  Dotations. Resources actually
*/
// Created: AGE 2006-02-13
// =============================================================================
class Dotations : public kernel::Dotations_ABC
                , public HierarchicExtension_ABC
                , public kernel::Updatable_ABC< sword::UnitAttributes >
                , public tools::Resolver< Dotation >
{
public:
    //! @name Constructors/Destructor
    //@{
             Dotations( kernel::Entity_ABC& entity, kernel::Controller& controller, const tools::Resolver_ABC< kernel::DotationType >& resolver, gui::PropertiesDictionary& dico
                      , const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~Dotations();
    //@}

    //! @name Operations
    //@{
    virtual bool Accept( const kernel::DotationType& type ) const;
    virtual bool IsEmptyGasTank() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Dotations( const Dotations& );            //!< Copy constructor
    Dotations& operator=( const Dotations& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& message );
    virtual void SetSuperior( const kernel::Entity_ABC& superior );
    void CreateDictionary( gui::PropertiesDictionary& dico ) const;
    void Update( const std::vector< Dotation >& differences );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::DotationType >& resolver_;
    gui::PropertiesDictionary& dictionary_;
    bool bEmptyGasTank_;
    const QString property_;
    //@}
};

#endif // __Dotations_h_
