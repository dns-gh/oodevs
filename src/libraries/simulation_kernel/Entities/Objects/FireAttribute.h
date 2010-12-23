// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __FireAttribute_h_
#define __FireAttribute_h_

#include "MIL.h"
#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "MIL_BurnEffectManipulator.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include <boost/shared_ptr.hpp>
#include <boost/serialization/export.hpp>

class MIL_FireClass;

// =============================================================================
/** @class  FireAttribute
    @brief  FireAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class FireAttribute
    : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FireAttribute();
    explicit FireAttribute( xml::xistream& xis );
    explicit FireAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~FireAttribute();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    //@}

    //@}

    //! @name Accessors
    //@{
    int GetInitialHeat() const;
    int GetDecreaseRate() const;
    int GetIncreaseRate();
    int GetMaxHeat() const;
    int GetCellSize() const;
    int GetMaxCombustionEnergy() const;
    int GetWeatherDecreateRate( const MIL_Object_ABC& object ) const;
    MIL_BurnEffectManipulator GetBurnEffect();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const MIL_FireClass* pClass_;
    int maxCombustionEnergy_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( FireAttribute )

#endif // __FireAttribute_h_
