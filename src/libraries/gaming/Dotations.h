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

#include "game_asn/Simulation.h"
#include "HierarchicExtension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class DotationType;
    class PropertiesDictionary;
    class Entity_ABC;
}

class Dotation;

// =============================================================================
/** @class  Dotations
    @brief  Dotations. Resources actually
*/
// Created: AGE 2006-02-13
// =============================================================================
class Dotations : public HierarchicExtension_ABC
                , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
                , public tools::Resolver< Dotation >
                , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Dotations( kernel::Controller& controller, const tools::Resolver_ABC< kernel::DotationType >& resolver, kernel::PropertiesDictionary& dico
                      , const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~Dotations();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Dotations( const Dotations& );            //!< Copy constructor
    Dotations& operator=( const Dotations& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    virtual void SetSuperior( const kernel::Entity_ABC& superior );
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    void Update( const std::vector< Dotation >& differences );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::DotationType >& resolver_;
    kernel::PropertiesDictionary& dictionary_;
    bool bEmptyGasTank_;
    //@}
};

#endif // __Dotations_h_
