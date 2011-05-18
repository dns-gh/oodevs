// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AltitudeModifierAttribute_h_
#define __AltitudeModifierAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  AltitudeModifierAttribute
    @brief  AltitudeModifierAttribute
*/
// Created: JSR 2011-05-17
// =============================================================================
class AltitudeModifierAttribute : public ObjectAttribute_ABC
                                , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AltitudeModifierAttribute();
    explicit AltitudeModifierAttribute( const xml::xistream& xis );
    explicit AltitudeModifierAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~AltitudeModifierAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Network updates
    //@{
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual void SendUpdate( sword::ObjectAttributes& asn ) const;
    //@}

    //! @name Copy/Assignment
    //@{
    AltitudeModifierAttribute& operator=( const AltitudeModifierAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AltitudeModifierAttribute( const AltitudeModifierAttribute& );            //!< Copy constructor
    //@}

private:
    //! @name Member data
    //@{
    unsigned int height_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( AltitudeModifierAttribute )

#endif // __AltitudeModifierAttribute_h_
