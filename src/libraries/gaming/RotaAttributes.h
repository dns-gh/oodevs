// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RotaAttributes_h_
#define __RotaAttributes_h_

#include "network/Simulation_Asn.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class NBCAgent;
}

// =============================================================================
/** @class  RotaAttributes
    @brief  RotaAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class RotaAttributes : public kernel::RotaAttributes_ABC
                     , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                     , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
                     , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             RotaAttributes( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::NBCAgent >& resolver );
    virtual ~RotaAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    RotaAttributes( const RotaAttributes& );            //!< Copy constructor
    RotaAttributes& operator=( const RotaAttributes& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::NBCAgent* > T_Nbcs;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::NBCAgent >& resolver_;
    kernel::OptionalValue< unsigned int > danger_;
    T_Nbcs agents_;
    //@}
};

#endif // __RotaAttributes_h_
