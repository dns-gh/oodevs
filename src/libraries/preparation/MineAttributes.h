// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MineAttributes_h_
#define __MineAttributes_h_

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
/** @class  MineAttributes
    @brief  MineAttributes
*/
// Created: SBO 2007-02-08
// =============================================================================
class MineAttributes : public kernel::MineAttributes_ABC
                     , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MineAttributes( kernel::PropertiesDictionary& dico );
             MineAttributes( xml::xistream& xis, kernel::PropertiesDictionary& dico );
    virtual ~MineAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
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
    MineAttributes( const MineAttributes& );            //!< Copy constructor
    MineAttributes& operator=( const MineAttributes& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< unsigned int > activityTime_;
    kernel::UnitedValue< double >       density_;
    //@}
};

#endif // __MineAttributes_h_
