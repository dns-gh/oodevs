// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __rpr_EntityIdentifier_h_
#define __rpr_EntityIdentifier_h_

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
