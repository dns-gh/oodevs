// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityType_h_
#define __EntityType_h_

// =============================================================================
/** @class  EntityType
    @brief  EntityTypeStruct complex data type
*/
// Created: AGE 2008-02-21
// =============================================================================
class EntityType
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityType();
    virtual ~EntityType();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EntityType( const EntityType& );            //!< Copy constructor
    EntityType& operator=( const EntityType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned char entityKind_;
    unsigned char domain_;
    unsigned short countryCode_;
    unsigned char category_;
    unsigned char subcategory_;
    unsigned char specific_;
    unsigned char extra_;
    //@}
};

#endif // __EntityType_h_
