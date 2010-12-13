// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SealOffAttribute_h_
#define __SealOffAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  SealOffAttribute
    @brief  SealOffAttribute
*/
// Created: MGD 2010-08-24
// =============================================================================
class SealOffAttribute : public ObjectAttribute_ABC
    , private UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    SealOffAttribute();
    virtual ~SealOffAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name Network Update
    //@{
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual void SendUpdate( sword::ObjectAttributes& asn ) const;
    //@}

    //! @name Operations
    //@{
    void SetSealOffLevel( float level );
    //@}

    //! @name Copy
    //@{
    SealOffAttribute& operator=( const SealOffAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    float rSealOffLevel_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( SealOffAttribute )

#endif // __SealOffAttribute_h_
