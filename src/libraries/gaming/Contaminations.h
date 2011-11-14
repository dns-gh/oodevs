// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Contaminations_h_
#define __Contaminations_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace sword
{
    class UnitAttributes;
}

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class NBCAgent;
    class PropertiesDictionary;
    class Entity_ABC;
}

// =============================================================================
/** @class  Contaminations
    @brief  Contaminations
*/
// Created: AGE 2006-02-13
// =============================================================================
class Contaminations : public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< sword::UnitAttributes >
                     , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Contaminations( kernel::Controller& controller, kernel::Entity_ABC& entity, const tools::Resolver_ABC< kernel::NBCAgent >& resolver, kernel::PropertiesDictionary& dico );
    virtual ~Contaminations();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Contaminations( const Contaminations& );            //!< Copy constructor
    Contaminations& operator=( const Contaminations& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::NBCAgent* > T_NbcAgents;
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    virtual void DoUpdate( const sword::UnitAttributes& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC& entity_;
    const tools::Resolver_ABC< kernel::NBCAgent >& resolver_;
    bool        bNbcProtectionSuitWorn_;
    T_NbcAgents contaminatingNbcAgents_;
    int         nContamination_;
    double      quantity_;
    //@}
};

#endif // __Contaminations_h_
