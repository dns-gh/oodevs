// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __BurnAttribute_h_
#define __BurnAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/serialization/export.hpp>

class FireAttribute;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  BurnAttribute
    @see "Incendie" in 4311340-Mod�les de feu.doc
*/
// Created: BCI 2010-12-09
// =============================================================================
class BurnAttribute : public ObjectAttribute_ABC
                    , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BurnAttribute();
    virtual ~BurnAttribute();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual void SendUpdate( sword::ObjectAttributes& asn ) const;
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void StartBurn( MIL_Object_ABC& object );
    void Burn( MIL_Object_ABC& object );
    //@}

    //! @name Operations
    //@{
    BurnAttribute( const BurnAttribute& ); //!< Copy operator
    BurnAttribute& operator=( const BurnAttribute& ); //!< Assignment operator
    bool Update( const BurnAttribute& rhs );    
    //@}

private:
    //! @name Member data
    //@{
    int currentHeat_;
    int combustionEnergySum_;
    int combustionEnergyCount_;
    int currentCombustionEnergy_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BurnAttribute )

#endif // __BurnAttribute_h_
