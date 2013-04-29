// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_ABC_h_
#define __MissionParameter_ABC_h_

#include "protocol/Protocol.h"

namespace kernel
{
    class OrderParameter;
}

namespace plugins
{
namespace bml
{

// =============================================================================
/** @class  MissionParameter_ABC
    @brief  Mission parameter declaration
*/
// Created: SBO 2008-05-22
// =============================================================================
class MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameter_ABC( const kernel::OrderParameter& type );
    virtual ~MissionParameter_ABC();
    //@}

    //! @name Accessors
    //@{
    const kernel::OrderParameter& GetType() const;
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( sword::MissionParameter& parameter ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_ABC( const MissionParameter_ABC& );            //!< Copy constructor
    MissionParameter_ABC& operator=( const MissionParameter_ABC& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& type_;
    //@}
};

}
}

#endif // __MissionParameter_ABC_h_