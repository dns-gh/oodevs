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
#include "MIL_FireClass.h"
#include <boost/shared_ptr.hpp>
#include <boost/serialization/export.hpp>

// =============================================================================
/** @class  FireAttribute
    @brief  FireAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class FireAttribute
    : public ObjectAttribute_ABC
    , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FireAttribute();
    explicit FireAttribute( xml::xistream& xis );
    explicit FireAttribute( const Common::MsgMissionParameter_Value& attributes );
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
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( Common::ObjectAttributes& asn ) const;
    virtual void SendUpdate( Common::ObjectAttributes& asn ) const;
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void ComputeHeatEvolution( unsigned int initial, unsigned int time );
    void ComputeHeatWhenExtinguishing( MIL_FireClass::E_FireExtinguisherAgent extinguisherAgent, int numberOfFireHoses );
    void UpdateHeat( int heat, unsigned int time );
    //@}

    //! @name Operations
    //@{
    int                      GetHeat() const;
    const MIL_FireClass&  GetClass() const;
    unsigned int          GetWidth() const;
    unsigned int          GetLength() const;
    //@}

    //! @name Operations
    //@{
    FireAttribute( const FireAttribute& ); //!< Copy operator
    FireAttribute& operator=( const FireAttribute& ); //!< Assignment operator
    bool Update( const FireAttribute& rhs );    
    //@}

private:
    //! @name Member data
    //@{
    //Fire temperature
    const MIL_FireClass*  pClass_;
    int                   heat_;
    unsigned int          timeOfLastUpdate_;
    unsigned int          width_;
    unsigned int          length_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( FireAttribute )

#endif // __FireAttribute_h_
