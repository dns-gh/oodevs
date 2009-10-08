// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAttribute_h_
#define __LogisticAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/StrongType.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/Resolver_ABC.h"
#include "Types.h"

namespace kernel
{
    class Automat_ABC;
    class Displayer_ABC;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  LogisticAttribute
    @brief  LogisticAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class LogisticAttribute : public kernel::LogisticAttribute_ABC
                        , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticAttribute( kernel::PropertiesDictionary& dico );
             LogisticAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::Automat_ABC >& automats, kernel::PropertiesDictionary& dico );
    virtual ~LogisticAttribute();
    //@}

    //! @name Operations
    //@{    
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void SetTC2( const kernel::Automat_ABC& tc2 );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticAttribute( const LogisticAttribute& );            //!< Copy constructor
    LogisticAttribute& operator=( const LogisticAttribute& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    TC2 tc2_;
    //@}
};

#endif // __LogisticAttribute_h_
