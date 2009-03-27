// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyRouteAttribute_h_
#define __SupplyRouteAttribute_h_

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
/** @class  SupplyRouteAttribute
    @brief  SupplyRouteAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class SupplyRouteAttribute : public kernel::SupplyRouteAttribute_ABC
                              , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SupplyRouteAttribute( kernel::PropertiesDictionary& dico );
             SupplyRouteAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico );
    virtual ~SupplyRouteAttribute();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetFlow( unsigned int value );
    void SetWidth( unsigned int value );
    void SetLength( unsigned int value );
    void SetMaxWeight( unsigned int value );
    void SetEquipped( bool value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SupplyRouteAttribute( const SupplyRouteAttribute& );            //!< Copy constructor
    SupplyRouteAttribute& operator=( const SupplyRouteAttribute& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

public:
    //! @name Member data
    //@{
    kernel::UnitedValue< unsigned int > flow_;
    kernel::UnitedValue< unsigned int > width_;
    kernel::UnitedValue< unsigned int > length_;
    kernel::UnitedValue< unsigned int > maxWeight_;
    bool equipped_;
    //@}
};

#endif // __SupplyRouteAttribute_h_
