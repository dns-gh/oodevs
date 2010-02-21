// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __StockAttribute_h_
#define __StockAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"


namespace kernel
{
    class Controller;
    class Automat_ABC;
    class Displayer_ABC;
    class DotationType;
}

namespace MsgsSimToClient
{
    class MsgObjectKnowledgeUpdate;
    class MsgObjectUpdate;
    class MsgObjectCreation;
}

namespace Common
{
    class StockResource;
}

// =============================================================================
/** @class  StockAttribute
    @brief  StockAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class StockAttribute : public kernel::StockAttribute_ABC
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeUpdate >
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectUpdate >
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             StockAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::DotationType >& resolver );
    virtual ~StockAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    StockAttribute( const StockAttribute& );            //!< Copy constructor
    StockAttribute& operator=( const StockAttribute& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef kernel::OptionalValue< unsigned int >   T_DotationState;
    typedef std::map< const kernel::DotationType*, std::pair< T_DotationState, T_DotationState > > T_StockDotation;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectCreation& message );
    
    template< typename T >
    void UpdateData( const T& message );
    void Update( const kernel::DotationType& type, const Common::StockResource& resource );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::DotationType >& resolver_;
    T_StockDotation stock_;
    //@}
};

#endif // __StockAttribute_h_
