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

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/shared_ptr.hpp>
#include "MIL_FireClass.h"

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
    explicit FireAttribute( const ASN1T_ObjectAttributes& asn );
    virtual ~FireAttribute();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name 
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    void SendFullState( ASN1T_ObjectAttributes& asn ) const;
    void SendUpdate( ASN1T_ObjectAttributes& asn ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

	//! @name Operations
    //@{
	void ComputeHeatEvolution( unsigned int initial, unsigned int time );
	void ComputeHeatWhenExtinguishing( MIL_FireClass::E_FireExtinguisherAgent extinguisherAgent, int numberOfFireHoses );
	void UpdateHeat( int heat, unsigned int time );	
	//@}

	//! @name Operations
    //@{
	int				      GetHeat() const;
    const MIL_FireClass&  GetClass() const;
	//@}

    //! @name Operations
    //@{
    FireAttribute( const FireAttribute& ); //!< Copy operator
    FireAttribute& operator=( const FireAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{    
	//Fire temperature    
    const MIL_FireClass*  pClass_;
    int                   heat_;
	unsigned int		  timeOfLastUpdate_;
    //@}
};

#endif // __FireAttribute_h_
