// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UndergroundAttribute_h_
#define __UndergroundAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  UndergroundAttribute
    @brief  UndergroundAttribute
*/
// Created: JSR 2011-07-11
// =============================================================================
class UndergroundAttribute : public ObjectAttribute_ABC
                           , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UndergroundAttribute();
    explicit UndergroundAttribute( xml::xistream& xis );
    explicit UndergroundAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~UndergroundAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
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
    virtual void OnUpdate( const sword::MissionParameter_Value& attribute );
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    //@}

    //! @name Getter/Setter
    //@{
    bool IsActivated() const;
    void SetActivation( bool activate );
    const std::string& Network() const;
    //@}

    //! @name Assignment
    //@{
    UndergroundAttribute& operator=( const UndergroundAttribute& ); //!< Assignment operator
    bool Update( const UndergroundAttribute& rhs );
    //@}

private:
    //! @name Copy
    //@{
    UndergroundAttribute( const UndergroundAttribute& );            //!< Copy constructor
    //@}

private:
    //! @name Member data
    //@{
    std::string network_;
    bool activated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( UndergroundAttribute )

#endif // __UndergroundAttribute_h_
