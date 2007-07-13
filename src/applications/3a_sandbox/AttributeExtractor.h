// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AttributeExtractor_h_
#define __AttributeExtractor_h_

#include "SimpleValueExtractor_ABC.h"

#define MAKE_EXTRACTOR( name, member )                            \
struct name                                                       \
{                                                                 \
    bool HasFlag ( const ASN1T_MsgUnitAttributes& attributes )    \
        { return attributes.m.member##Present; }                  \
    float Extract( const ASN1T_MsgUnitAttributes& attributes )    \
        { return float( attributes.member ); }                    \
};

namespace extractors
{
    MAKE_EXTRACTOR( OpState, etat_operationnel_brut );
    MAKE_EXTRACTOR( Heading, direction );
    MAKE_EXTRACTOR( Speed,   vitesse );

    struct Humans
    {
        Humans( ASN1T_EnumHumanRank rank, ASN1INT ASN1T_HumanDotations::* status )
            : rank_( rank )
            , status_( status ) {}

        bool HasFlag ( const ASN1T_MsgUnitAttributes& attributes )
        {
            if( attributes.m.dotation_eff_personnelPresent )
            {
                for( unsigned i = 0; i < attributes.dotation_eff_personnel.n; ++i )
                    if( attributes.dotation_eff_personnel.elem[i].rang == rank_ )
                        return true;
            }
            return false;
        }
        float Extract( const ASN1T_MsgUnitAttributes& attributes )
        {
            for( unsigned i = 0; i < attributes.dotation_eff_personnel.n; ++i )
                if( attributes.dotation_eff_personnel.elem[i].rang == rank_ )
                    return float( attributes.dotation_eff_personnel.elem[i].*status_ );
            return 0.f;
        }
        ASN1T_EnumHumanRank             rank_;
        ASN1INT ASN1T_HumanDotations::* status_;
    };
}

// =============================================================================
/** @class  AttributeExtractor
    @brief  AttributeExtractor
*/
// Created: AGE 2007-07-12
// =============================================================================
template< typename Extractor >
class AttributeExtractor : public SimpleValueExtractor_ABC
{
public:
    //! @name Constructor
    //@{
    explicit AttributeExtractor( const Extractor& extractor = Extractor() )
        : extractor_( extractor ) {}
    //@}

    //! @name Operations
    //@{
    virtual float ExtractValue( const ASN1T_MsgsSimToClient& message )
    {
        if( message.msg.t == T_MsgsSimToClient_msg_msg_unit_attributes )
        {
            const ASN1T_MsgUnitAttributes& attributes = *message.msg.u.msg_unit_attributes;
            if( extractor_.HasFlag( attributes ) )
                return extractor_.Extract( attributes );
        }
        return Noop();
    }
    //@}

private:
    //! @name Member data
    //@{
    Extractor extractor_;
    //@}
};

#endif // __AttributeExtractor_h_
