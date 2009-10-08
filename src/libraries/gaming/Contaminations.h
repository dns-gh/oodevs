// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Contaminations_h_
#define __Contaminations_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class NBCAgent;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  Contaminations
    @brief  Contaminations
*/
// Created: AGE 2006-02-13
// =============================================================================
class Contaminations : public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
                     , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Contaminations( kernel::Controller& controller, const tools::Resolver_ABC< kernel::NBCAgent >& resolver, kernel::PropertiesDictionary& dico );
    virtual ~Contaminations();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Contaminations( const Contaminations& );            //!< Copy constructor
    Contaminations& operator=( const Contaminations& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::NBCAgent* > T_NbcAgents;
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::NBCAgent >& resolver_;
    bool        bNbcProtectionSuitWorn_;
    T_NbcAgents contaminatingNbcAgents_;
    int         nContamination_;
    double      quantity_;
    //@}
};

#endif // __Contaminations_h_
