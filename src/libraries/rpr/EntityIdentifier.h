// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __rpr_EntityIdentifier_h_
#define __rpr_EntityIdentifier_h_

#include <string>

namespace rpr
{
// =============================================================================
/** @class  EntityIdentifier
    @brief  EntityIdentifierStruct complex data type
*/
// Created: AGE 2008-02-21
// =============================================================================
class EntityIdentifier
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityIdentifier();
             EntityIdentifier( unsigned short site, unsigned short application, unsigned short number );
    virtual ~EntityIdentifier();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        federateIdentifier_.Serialize( archive );
        archive << entityNumber_;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        federateIdentifier_.Deserialize( archive );
        archive >> entityNumber_;
    }

    bool operator == ( const EntityIdentifier& other ) const
    {
        return entityNumber_ == other.entityNumber_ &&
                federateIdentifier_.siteID_ == other.federateIdentifier_.siteID_ &&
                federateIdentifier_.applicationID_ == other.federateIdentifier_.applicationID_;
    }

    std::string str() const;
    //@}

private:
    //! @name Types
    //@{
    struct FederateIdentifier
    {
        FederateIdentifier( unsigned short site, unsigned short application )
            : siteID_( site ), applicationID_( application ) {}
        template< typename Archive >
        void Serialize( Archive& archive ) const
        {
            archive << siteID_ << applicationID_;
        }
        template< typename Archive >
        void Deserialize( Archive& archive )
        {
            archive >> siteID_ >> applicationID_;
        }
        unsigned short siteID_;
        unsigned short applicationID_;
    };
    //@}

private:
    //! @name Member data
    //@{
    FederateIdentifier federateIdentifier_;
    unsigned short entityNumber_;
    //@}
};

}

#endif // __rpr_EntityIdentifier_h_
