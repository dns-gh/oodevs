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
#include <boost/serialization/export.hpp>

struct ASN1T_ObjectAttributes;

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
    explicit InteractionHeightAttribute( const ASN1T_ObjectAttributes& asn );
    explicit InteractionHeightAttribute( xml::xistream& xis );
    virtual ~InteractionHeightAttribute();
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Knowledge instance
    //@{    
    virtual void Instanciate( DEC_Knowledge_Object& /*object*/ ) const;
    //@}

    //! @name ASN Network update
    //@{     
    virtual void SendFullState( ASN1T_ObjectAttributes& /*asn*/ ) const; //<! Send update to ASN client
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& /*xos*/ ) const;
    //@}

    //! @name Accessors
    //@{
    MT_Float Get() const;
    //@}

private:
    //! @name Member data
    //@{
    MT_Float height_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( InteractionHeightAttribute )

#endif // __InteractionHeightAttribute_h_
