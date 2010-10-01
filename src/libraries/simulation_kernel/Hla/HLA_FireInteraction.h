// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HLA_FireInteraction_h_
#define __HLA_FireInteraction_h_

class PHY_DotationCategory;

namespace hla
{
    template < typename T > class Interaction;
}

// =============================================================================
/** @class  HLA_FireInteraction
    @brief  HLA Fire interaction
*/
// Created: AGE 2004-11-29
// =============================================================================
class HLA_FireInteraction
{
public:
    //! @name Constructors/Destructor
    //@{
             HLA_FireInteraction();
    explicit HLA_FireInteraction( const PHY_DotationCategory& ammunition );
    virtual ~HLA_FireInteraction();
    //@}

protected:
    //! @name Helpers
    //@{
    const PHY_DotationCategory* GetAmmunition() const;

    template< typename SubClass >
    static void RegisterAttributes( hla::Interaction< SubClass >& interaction );
    //@}

private:
    //! @name Member data
    //@{
    std::string strAmmunition_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: HLA_FireInteraction::RegisterAttributes
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
template< typename SubClass >
void HLA_FireInteraction::RegisterAttributes( hla::Interaction< SubClass >& interaction )
{
    interaction.Register( hla::ParameterIdentifier( "munition" ), *new hla::Encodable< SubClass, std::string >( & HLA_FireInteraction::strAmmunition_ ) );
}

#endif // __HLA_FireInteraction_h_
