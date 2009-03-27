// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSiteAttribute_h_
#define __CrossingSiteAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"

class CrossingSiteAttribute 
    : public ObjectAttribute_ABC
    , public UpdatableAttribute_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
	         CrossingSiteAttribute();
    explicit CrossingSiteAttribute( xml::xistream& xis );
    explicit CrossingSiteAttribute( const ASN1T_ObjectAttributes& asn );
	virtual ~CrossingSiteAttribute();
	//@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name ODB
    //@{
    /*final*/ void WriteODB( xml::xostream& /*xos*/ ) const;    
    //@}

    //! @name Network
    //@{
    void SendFullState( ASN1T_ObjectAttributes& asn ) const;
    void SendUpdate( ASN1T_ObjectAttributes& asn ) const;
    void OnUpdate( const ASN1T_ObjectAttributes& asn );    
    //@}

    //! @name Knowledge
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name Accessors
    //@{
    MT_Float GetWidth() const;
    MT_Float GetDepth() const;
    MT_Float GetSpeed() const;
    bool     IsBanksToFitOut() const;
    //@}

    //! @name Copy
    //@{
    CrossingSiteAttribute& operator=( const CrossingSiteAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy
    //@{
    CrossingSiteAttribute( const CrossingSiteAttribute& from );
    //@}

private:
    //! @name 
    //@{
    MT_Float rWidth_;
    MT_Float rDepth_;
    MT_Float rCurrentSpeed_;  // Vitesse du courant
    bool     bBanksToFitOut_; // Berges a amenager
    //@}
};

#endif // __CrossingSiteAttribute_h_
