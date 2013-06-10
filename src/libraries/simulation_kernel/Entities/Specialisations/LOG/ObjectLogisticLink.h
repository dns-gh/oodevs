// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ObjectLogisticLink_h_
#define __ObjectLogisticLink_h_

#include "MIL.h"

#include "LogisticLink_ABC.h"

namespace logistic {

// =============================================================================
/** @class  ObjectLogisticLink
    @brief  ObjectLogisticLink
*/
// Created: NLD 2011-01-05
// =============================================================================
class ObjectLogisticLink : public LogisticLink_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectLogisticLink( MIL_AutomateLOG& superior );
    virtual ~ObjectLogisticLink();
    //@}

    //! @name Operations
    //@{
    virtual MIL_AutomateLOG& GetSuperior() const;

    virtual double ConsumeQuota( const PHY_DotationCategory& dotationCategory, double quantity, const T_Requesters& requesters );
    virtual void   ReturnQuota ( const PHY_DotationCategory& dotationCategory, double quantity );
    //@}

    //! @name Serialization
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Network
    //@{
    virtual void SendFullState   ( unsigned int context ) const;
    virtual void SendChangedState() const;

    virtual std::set< const PHY_DotationCategory* > OnReceiveChangeQuotas( const sword::MissionParameter& message );
    //@}

private:
    MIL_AutomateLOG* superior_;

private:
    template< typename Archive > friend  void save_construct_data( Archive& archive, const ObjectLogisticLink*, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, ObjectLogisticLink*, const unsigned int /*version*/ );
};

} // namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::ObjectLogisticLink );

#endif // __LogisticLink_h_
