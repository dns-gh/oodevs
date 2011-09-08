// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DelayAttribute_h_
#define __DelayAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  DelayAttribute
    @brief  DelayAttribute
*/
// Created: JSR 2010-07-06
// =============================================================================
class DelayAttribute : public ObjectAttribute_ABC
                     , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DelayAttribute();
    explicit DelayAttribute( xml::xistream& xis );
    explicit DelayAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~DelayAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
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
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    //@}

    //! @name Operation
    //@{
    bool IsOverDelay( unsigned int nTicks ) const;
    //@}

    //! @name Assignment
    //@{
    DelayAttribute& operator=( const DelayAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy
    //@{
    DelayAttribute( const DelayAttribute& );            //!< Copy constructor
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nDelay_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DelayAttribute )

#endif // __DelayAttribute_h_
