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

#include "ASN_Types.h"
#include "astec_kernel/Extension_ABC.h"
#include "astec_kernel/Updatable_ABC.h"
#include "astec_kernel/Resolver_ABC.h"
#include "astec_kernel/Drawable_ABC.h"

class Controller;
class Displayer_ABC;
class NBCAgent;
class DataDictionary;

// =============================================================================
/** @class  Contaminations
    @brief  Contaminations
*/
// Created: AGE 2006-02-13
// =============================================================================
class Contaminations : public Extension_ABC
                     , public Updatable_ABC< ASN1T_MsgUnitAttributes >
                     , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Contaminations( Controller& controller, const Resolver_ABC< NBCAgent >& resolver, DataDictionary& dico );
    virtual ~Contaminations();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Contaminations( const Contaminations& );            //!< Copy constructor
    Contaminations& operator=( const Contaminations& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const NBCAgent* > T_NbcAgents;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

public:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< NBCAgent >& resolver_;
    bool        bNbcProtectionSuitWorn_;
    T_NbcAgents contaminatingNbcAgents_;
    int         nContamination_;
    //@}
};

#endif // __Contaminations_h_
