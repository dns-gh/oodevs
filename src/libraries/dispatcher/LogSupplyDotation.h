 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogSupplyDotation_h_
#define __LogSupplyDotation_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class MsgDotationQuery;
}

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  LogSupplyDotation
    @brief  LogSupplyDotation
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogSupplyDotation : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogSupplyDotation( const Model& model, const sword::MsgDotationQuery& asnMsg );
    virtual ~LogSupplyDotation();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::MsgDotationQuery& asnMsg );
    void Send  ( sword::MsgDotationQuery& asnMsg ) const ;
    //@}

private:
    const unsigned int nDotationType_; // XML reference - no resolved by dispatcher
    unsigned int nNbrRequested_;
    unsigned int nNbrGranted_;
    unsigned int nNbrConvoyed_;
};

}

#endif // __LogSupplyDotation_h_
