// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyRouteAttribute_h_
#define __SupplyRouteAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/serialization/export.hpp>

// =============================================================================
/** @class  SupplyRouteAttribute
    @brief  SupplyRouteAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class SupplyRouteAttribute : public ObjectAttribute_ABC
                           , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyRouteAttribute();
    explicit SupplyRouteAttribute( xml::xistream& xis );
    explicit SupplyRouteAttribute( const Common::MsgObjectAttributes& asn );
    virtual ~SupplyRouteAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& /*xos*/ ) const;    
    //@}

    //! @name Network
    //@{
    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    void Serialize ( HLA_UpdateFunctor& functor ) const;
    virtual void SendFullState( Common::MsgObjectAttributes& asn ) const;
    virtual void SendUpdate( Common::MsgObjectAttributes& asn ) const;
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name Accessors
    //@{
    void        Equip();
    bool        IsEquipped() const;
    MT_Float    GetMaxWeight() const;
    MT_Float    GetWidth() const;
    MT_Float    GetLength() const;
    MT_Float    GetFlow() const;
    //@}

    //! @name Copy
    //@{
    SupplyRouteAttribute& operator=( const SupplyRouteAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy
    //@{
    SupplyRouteAttribute( const SupplyRouteAttribute& from );
    //@}

private:
    //! @name 
    //@{
    bool       bEquipped_;
    MT_Float   rWeightSupported_;
    MT_Float   rWidth_;
    MT_Float   rLength_;
    MT_Float   rFlow_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( SupplyRouteAttribute )

#endif
