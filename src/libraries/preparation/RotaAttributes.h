// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RotaAttributes_h_
#define __RotaAttributes_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class NBCAgent;
}

// =============================================================================
/** @class  RotaAttributes
    @brief  RotaAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class RotaAttributes : public kernel::RotaAttributes_ABC
                     , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RotaAttributes();
    virtual ~RotaAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetDanger( unsigned int level );
    void AddAgent( const kernel::NBCAgent& agent );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    RotaAttributes( const RotaAttributes& );            //!< Copy constructor
    RotaAttributes& operator=( const RotaAttributes& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::NBCAgent* > T_Nbcs;
    //@}

public:
    //! @name Member data
    //@{
    kernel::OptionalValue< unsigned int > danger_;
    T_Nbcs agents_;
    //@}
};

#endif // __RotaAttributes_h_
