// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MineAttribute_h_
#define __MineAttribute_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/OptionalValue.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class DotationType;
}

// =============================================================================
/** @class  MineAttribute
    @brief  MineAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class MineAttribute : public kernel::MineAttribute_ABC
                     , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                     , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
                     , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
            MineAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::DotationType >& resolver, bool minefield = false );
    virtual ~MineAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MineAttribute( const MineAttribute& );            //!< Copy constructor
    MineAttribute& operator=( const MineAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::DotationType >& resolver_;

    kernel::OptionalValue< unsigned int > nDotationValorization_;
    kernel::DotationType*                 valorization_;
    kernel::OptionalValue< float >        rValorizationPercentage_;
    
    bool minefield_;
    kernel::OptionalValue< unsigned int > activityTime_;
    kernel::OptionalValue< float >        density_;    
    //@}
};

#endif // __MineAttribute_h_
