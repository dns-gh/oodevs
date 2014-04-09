// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConsign_ABC_h_
#define __SupplyConsign_ABC_h_

#include <boost/enable_shared_from_this.hpp>

class PHY_DotationCategory;
class MIL_AutomateLOG;
class MIL_Agent_ABC;
class MIL_AgentPion;

namespace logistic
{
    class SupplyRequest_ABC;
    class SupplyRecipient_ABC;
    class SupplyConvoy_ABC;

// =============================================================================
/** @class  SupplyConsign_ABC
    @brief  SupplyConsign_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConsign_ABC : public boost::enable_shared_from_this< SupplyConsign_ABC >
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConsign_ABC() {}
    virtual ~SupplyConsign_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void AddRequest( SupplyRecipient_ABC& recipient, boost::shared_ptr< SupplyRequest_ABC > request ) = 0;
    virtual void Activate  () = 0;
    virtual bool Update    () = 0;
    virtual bool IsSupplying( const PHY_DotationCategory& dotationCategory, const SupplyRecipient_ABC& recipient ) = 0;
    virtual bool IsFinished() const = 0;
    virtual bool ResetConsignsForConvoyPion( const MIL_Agent_ABC& pion ) = 0;
    virtual bool ResetConsignsForProvider( const MIL_Agent_ABC& pion ) = 0;
    virtual void ResetConsign() = 0;
    virtual bool GrantsNothing() const = 0;
    virtual void FinishSuccessfullyWithoutDelay() = 0;
    //@}

    //! @name TMP - A remonter dans une autre interface ?
    //$$$$ Pour chaine mortuaire
    //@{
    virtual bool WillGoTo( const MIL_AutomateLOG& destination ) const = 0;
    virtual bool IsAt    ( const MIL_AutomateLOG& destination ) const = 0;
    virtual boost::shared_ptr< SupplyConvoy_ABC > GetConvoy() const = 0;
    //@}

    //! @name Network - A refactorer
    //@{
    virtual void SendChangedState() const = 0;
    virtual void SendFullState   () const = 0;
    virtual void Clean           () = 0;    
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

} // end namespace logistic

#endif // __SupplyConsign_ABC_h_
