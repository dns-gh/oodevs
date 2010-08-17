// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Savable_ABC_h_
#define __Savable_ABC_h_

namespace tools
{
    class OutputBinaryWrapper;
}

namespace plugins
{
namespace saver
{
// =============================================================================
/** @class  Savable_ABC
    @brief  Savable_ABC
*/
// Created: AGE 2007-04-10
// =============================================================================
class Savable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Savable_ABC() {}
    virtual ~Savable_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( tools::OutputBinaryWrapper& output ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Savable_ABC( const Savable_ABC& );            //!< Copy constructor
    Savable_ABC& operator=( const Savable_ABC& ); //!< Assignment operator
    //@}
};

}
}

#endif // __Savable_ABC_h_
