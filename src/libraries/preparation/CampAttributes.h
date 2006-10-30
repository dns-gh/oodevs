// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CampAttributes_h_
#define __CampAttributes_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/StrongType.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "Types.h"

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Displayer_ABC;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  CampAttributes
    @brief  CampAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class CampAttributes : public kernel::CampAttributes_ABC
                     , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CampAttributes( kernel::Controllers& controllers, kernel::PropertiesDictionary& dico );
             CampAttributes( xml::xistream& xis, kernel::Controllers& controllers, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, kernel::PropertiesDictionary& dico );
    virtual ~CampAttributes();
    //@}

    //! @name Operations
    //@{
    void SetTC2( const kernel::Automat_ABC& tc2 );
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CampAttributes( const CampAttributes& );            //!< Copy constructor
    CampAttributes& operator=( const CampAttributes& ); //!< Assignement operator
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

#endif // __CampAttributes_h_
