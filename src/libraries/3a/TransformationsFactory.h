// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TransformationsFactory_h_
#define __TransformationsFactory_h_

#include "ElementFactory_ABC.h"

// =============================================================================
/** @class  TransformationsFactory
    @brief  TransformationsFactory
*/
// Created: AGE 2008-08-04
// =============================================================================
class TransformationsFactory : public ElementFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TransformationsFactory();
    virtual ~TransformationsFactory();
    //@}

    //! @name Operations
    //@{
    virtual void CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TransformationsFactory( const TransformationsFactory& );            //!< Copy constructor
    TransformationsFactory& operator=( const TransformationsFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    static void Error( const std::string& name );
    template< typename T >
    void Compose( const std::string& name, xml::xistream& xis, Task& result ) const;
    template< typename T >
    void Transform( const std::string& name, xml::xistream& xis, Task& result ) const;
    template< typename T >
    void Transform1( const std::string& name, xml::xistream& xis, Task& result ) const;
    template< typename T >
    void Transform2( const std::string& name, xml::xistream& xis, Task& result ) const;
    struct TransformDispatcher;
    //@}
};

#endif // __TransformationsFactory_h_
