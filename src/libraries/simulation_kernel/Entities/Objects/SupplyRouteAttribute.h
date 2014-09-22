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
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

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
    explicit SupplyRouteAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~SupplyRouteAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& /*xos*/ ) const;
    //@}

    //! @name Network
    //@{
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name Accessors
    //@{
    void Equip();
    bool IsEquipped() const;
    double GetMaxWeight() const;
    double GetWidth() const;
    double GetLength() const;
    double GetFlow() const;
    //@}

    //! @name Copy
    //@{
    SupplyRouteAttribute& operator=( const SupplyRouteAttribute& ); //!< Assignment operator
    bool Update( const SupplyRouteAttribute& rhs );
    //@}

private:
    //! @name Copy
    //@{
    SupplyRouteAttribute( const SupplyRouteAttribute& from );
    //@}

private:
    //! @name
    //@{
    bool bEquipped_;
    double rWeightSupported_;
    double rWidth_;
    double rLength_;
    double rFlow_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( SupplyRouteAttribute )

#endif
