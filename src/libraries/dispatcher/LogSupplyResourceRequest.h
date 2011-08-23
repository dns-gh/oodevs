 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogSupplyResourceRequest_h_
#define __LogSupplyResourceRequest_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class SupplyResourceRequest;
}

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  LogSupplyResourceRequest
    @brief  LogSupplyResourceRequest
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogSupplyResourceRequest : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogSupplyResourceRequest( const Model& model, const sword::SupplyResourceRequest& asnMsg );
    virtual ~LogSupplyResourceRequest();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::SupplyResourceRequest& msg );
    void Send  ( sword::SupplyResourceRequest& msg ) const ;
    //@}

private:
    const unsigned int nDotationType_; // XML reference - no resolved by dispatcher
    unsigned int nNbrRequested_;
    unsigned int nNbrGranted_;
    unsigned int nNbrConvoyed_;
};

}

#endif // __LogSupplyResourceRequest_h_
