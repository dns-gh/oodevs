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
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include <boost/serialization/export.hpp>

class TER_Localisation;

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
    explicit AltitudeModifierAttribute( const sword::MissionParameter_Value& attributes, const TER_Localisation& localisation, unsigned int objectId );
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
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Network updates
    //@{
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    //@}

    //! @name Copy/Assignment
    //@{
    AltitudeModifierAttribute& operator=( const AltitudeModifierAttribute& ); //!< Assignment operator
    bool Update( const AltitudeModifierAttribute& rhs );
    //@}

    //! @name Operations
    //@{
    void ModifyAltitude( const TER_Localisation& localisation, unsigned int objectId ) const;
    void ResetAltitude( const TER_Localisation& localisation, unsigned int objectId ) const;
    void SetHeight( unsigned int height );
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
