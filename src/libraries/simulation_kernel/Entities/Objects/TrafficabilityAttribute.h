// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __TrafficabilityAttribute_h_
#define __TrafficabilityAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  TrafficabilityAttribute
    @brief  Trafficability attribute
*/
// Created: LGY 2011-08-23
// =============================================================================
class TrafficabilityAttribute : public ObjectAttribute_ABC
                              , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TrafficabilityAttribute();
    explicit TrafficabilityAttribute( xml::xistream& xis );
    virtual ~TrafficabilityAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const double );
    //@}

    //! @name Operations
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual void SendUpdate( sword::ObjectAttributes& asn ) const;

    virtual void OnUpdate( const sword::MissionParameter_Value& attribute );
    //@}

    //! @name Copy/Assignment
    //@{
    TrafficabilityAttribute& operator=( const TrafficabilityAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TrafficabilityAttribute( const TrafficabilityAttribute& );            //!< Copy constructorr
    //@}

private:
    //! @name Member data
    //@{
    double max_;
    //@}
};

#endif // __TrafficabilityAttribute_h_
