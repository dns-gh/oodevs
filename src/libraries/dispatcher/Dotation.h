 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dotation_h_
#define __Dotation_h_

namespace Common
{
    class MsgDotationStock;
}

namespace MsgsSimToClient
{
    class ResourceDotations_ResourceDotation;
}

namespace dispatcher
{

// =============================================================================
/** @class  Dotation
    @brief  Dotation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Dotation
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Dotation( const MsgsSimToClient::ResourceDotations_ResourceDotation& asnMsg );
    explicit Dotation( const Common::MsgDotationStock    & asnMsg );
    virtual ~Dotation();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsSimToClient::ResourceDotations_ResourceDotation& asnMsg );
    void Update( const Common::MsgDotationStock& asnMsg );
    void Send  ( MsgsSimToClient::ResourceDotations_ResourceDotation& asnMsg ) const ;
    void Send  ( Common::MsgDotationStock& asnMsg ) const ;
    //@}

private:
    const unsigned int nDotationType_; // XML reference - no resolved by dispatcher
          unsigned int nNbr_;
};

}

#endif // __Dotation_h_
