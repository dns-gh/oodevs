// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityIdentifier_h_
#define __EntityIdentifier_h_

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
    virtual ~EntityIdentifier();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EntityIdentifier( const EntityIdentifier& );            //!< Copy constructor
    EntityIdentifier& operator=( const EntityIdentifier& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    struct FederateIdentifier
    {
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

#endif // __EntityIdentifier_h_
