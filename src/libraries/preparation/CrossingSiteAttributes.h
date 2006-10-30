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
#include "clients_kernel/Units.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
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
    explicit CrossingSiteAttributes( kernel::PropertiesDictionary& dico );
             CrossingSiteAttributes( xml::xistream& xis, kernel::PropertiesDictionary& dico );
    virtual ~CrossingSiteAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
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

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

public:
    //! @name Member data
    //@{
    kernel::UnitedValue< unsigned int > width_;
    kernel::UnitedValue< unsigned int > depth_;
    kernel::UnitedValue< unsigned int > speed_;
    bool needsConstruction_;
    //@}
};

#endif // __CrossingSiteAttributes_h_
