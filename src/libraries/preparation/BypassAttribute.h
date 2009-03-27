// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BypassAttribute_h_
#define __BypassAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Units.h"

namespace kernel
{
    class Displayer_ABC;
    class NBCAgent;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  BypassAttribute
    @brief  BypassAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class BypassAttribute : public kernel::BypassAttribute_ABC
                      , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit BypassAttribute( kernel::PropertiesDictionary& dico );
             BypassAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico );
    virtual ~BypassAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetActivityTime( unsigned int time );
    void SetDensity( float density );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BypassAttribute( const BypassAttribute& );            //!< Copy constructor
    BypassAttribute& operator=( const BypassAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< float > rBypassConstructionPercentage_;
    //@}
};

#endif // __BypassAttribute_h_
