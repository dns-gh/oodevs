// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectType_h_
#define __ObjectType_h_

namespace xml { class xistream; };

namespace kernel
{
// =============================================================================
/** @class  ObjectType
    @brief  ObjectType
*/
// Created: AGE 2006-02-16
// =============================================================================
class ObjectType
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectType( xml::xistream& xis, unsigned long id );
    virtual ~ObjectType();
    //@}

    //! @name Operations
    //@{
    std::string GetName() const;

    bool CanBeReservedObstacle() const;
    bool CanBeValorized() const;
    bool CanBeBypassed() const;

    unsigned long GetId() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectType( const ObjectType& );            //!< Copy constructor
    ObjectType& operator=( const ObjectType& ); //!< Assignement operator
    //@}

public:
    //! @name Static members
    //@{
    unsigned long id_;
    std::string name_;

    bool canBeReservedObstacle_;
    bool canBeValorized_;
    bool canBeBypassed_;
    //@}
};

}

#endif // __ObjectType_h_
