// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSiteAttributes_h_
#define __CrossingSiteAttributes_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Displayer_ABC;
}

// =============================================================================
/** @class  CrossingSiteAttributes
    @brief  CrossingSiteAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class CrossingSiteAttributes : public kernel::CrossingSiteAttributes_ABC
                             , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CrossingSiteAttributes();
    virtual ~CrossingSiteAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetWidth( unsigned int value );
    void SetDepth( unsigned int value );
    void SetFlowSpeed( unsigned int value );
    void SetConstruction( bool need );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CrossingSiteAttributes( const CrossingSiteAttributes& );            //!< Copy constructor
    CrossingSiteAttributes& operator=( const CrossingSiteAttributes& ); //!< Assignement operator
    //@}

public:
    //! @name Member data
    //@{
    kernel::OptionalValue< unsigned int > width_;
    kernel::OptionalValue< unsigned int > depth_;
    kernel::OptionalValue< unsigned int > speed_;
    kernel::OptionalValue< bool         > needsConstruction_;
    //@}
};

#endif // __CrossingSiteAttributes_h_
