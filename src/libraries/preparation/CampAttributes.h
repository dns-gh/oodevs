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
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Displayer_ABC;
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
    explicit CampAttributes( kernel::Controllers& controllers );
    virtual ~CampAttributes();
    //@}

    //! @name Operations
    //@{
    void SetTC2( const kernel::Automat_ABC& tc2 );
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CampAttributes( const CampAttributes& );            //!< Copy constructor
    CampAttributes& operator=( const CampAttributes& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Automat_ABC > tc2_;
    //@}
};

#endif // __CampAttributes_h_
