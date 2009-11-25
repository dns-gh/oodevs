// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IdentifierFactory_h_
#define __IdentifierFactory_h_

// =============================================================================
/** @class  IdentifierFactory
    @brief  IdentifierFactory
*/
// Created: SBO 2009-11-16
// =============================================================================
class IdentifierFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             IdentifierFactory();
    virtual ~IdentifierFactory();
    //@}

    //! @name Operations
    //@{
    unsigned long Create();
    void Reserve( unsigned long id );
    void Reset();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IdentifierFactory( const IdentifierFactory& );            //!< Copy constructor
    IdentifierFactory& operator=( const IdentifierFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned long last_;
    //@}
};

#endif // __IdentifierFactory_h_
