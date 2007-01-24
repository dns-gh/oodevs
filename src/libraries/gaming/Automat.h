// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "network/Simulation_Asn.h"

namespace kernel
{
    class Controller;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat : public kernel::EntityImplementation< kernel::Automat_ABC >
              , public kernel::Extension_ABC
              , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Automat( const ASN1T_MsgAutomateCreation& message, kernel::Controller& controller, 
                      const kernel::Resolver_ABC< kernel::AutomatType >& resolver );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AutomatType& GetType() const;
    //@}

    //! @name Extensions
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::AutomatType& type_;
    //@}
};

#endif // __Automat_h_
