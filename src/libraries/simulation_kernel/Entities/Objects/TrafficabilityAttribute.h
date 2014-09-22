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
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include <boost/serialization/export.hpp>

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
    explicit TrafficabilityAttribute( const sword::MissionParameter_Value& attributes );
    explicit TrafficabilityAttribute( float max );
    explicit TrafficabilityAttribute( xml::xistream& xis );
    virtual ~TrafficabilityAttribute();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Accessors
    //@{
    float GetMaxValue() const;
    //@}

    //! @name Operations
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    virtual void OnUpdate( const sword::MissionParameter_Value& attribute );
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name Copy/Assignment
    //@{
    TrafficabilityAttribute& operator=( const TrafficabilityAttribute& ); //!< Assignment operator
    bool Update( const TrafficabilityAttribute& rhs );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TrafficabilityAttribute( const TrafficabilityAttribute& );            //!< Copy constructorr
    //@}

private:
    //! @name Member data
    //@{
    float max_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( TrafficabilityAttribute )

#endif // __TrafficabilityAttribute_h_
