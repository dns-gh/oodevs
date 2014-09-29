// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __InteractionHeightAttribute_h_
#define __InteractionHeightAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  InteractionHeightAttribute
    @brief  InteractionHeightAttribute
*/
// Created: LDC 2009-03-09
// =============================================================================
class InteractionHeightAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InteractionHeightAttribute();
             InteractionHeightAttribute( double height );
    explicit InteractionHeightAttribute( const sword::MissionParameter_Value& attributes );
    explicit InteractionHeightAttribute( xml::xistream& xis );
    virtual ~InteractionHeightAttribute();
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Knowledge instance
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name ASN Network update
    //@{
    virtual void SendFullState( sword::ObjectAttributes& asn ) const; //<! Send update to asn client
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& /*xos*/ ) const;
    //@}

    //! @name Accessors
    //@{
    double Get() const;
    //@}

    //! @name Copy
    //@{
    bool Update( const InteractionHeightAttribute& rhs );
    //@}

private:
    //! @name Member data
    //@{
    double height_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( InteractionHeightAttribute )

#endif // __InteractionHeightAttribute_h_
