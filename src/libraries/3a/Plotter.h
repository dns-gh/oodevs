// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Plotter_h_
#define __Plotter_h_

#include "dispatcher/ClientPublisher_ABC.h"
#include "ValueHandler_ABC.h"
#include "Result_ABC.h"

// =============================================================================
/** @class  Plotter
    @brief  Plotter
*/
// Created: AGE 2007-09-12
// =============================================================================
template< typename T >
class Plotter : public ValueHandler_ABC< T >
              , public Result_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Plotter() {};
    virtual ~Plotter() {};
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Plotter"; }
    virtual void Handle( const T& value )
    {
        values_.push_back( value );
    }
    virtual void Send( dispatcher::ClientPublisher_ABC& publisher ) const
    {
        std::vector< double > values; values.reserve( values_.size() );
        std::copy( values_.begin(), values_.end(), std::back_inserter( values ) );

        ASN1T_MsgIndicatorResult result;
        result.values.n    =  values.size();
        result.values.elem = &values.front();

        ASN1T_MsgsAfterActionReviewToClient message;
//        message.context = $$$$;
        message.msg.t = T_MsgsAfterActionReviewToClient_msg_msg_indicator_result;
        message.msg.u.msg_indicator_result = &result;

        publisher.Send( message );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Plotter( const Plotter& );            //!< Copy constructor
    Plotter& operator=( const Plotter& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< T > T_Values;
    //@}

private:
    //! @name Member data
    //@{
    T_Values values_;
    //@}
};

#endif // __Plotter_h_
