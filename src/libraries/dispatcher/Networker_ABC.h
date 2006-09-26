// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Networker_ABC_h_
#define __Networker_ABC_h_

namespace dispatcher 
{
class Dispatcher;

// =============================================================================
/** @class  Networker_ABC
    @brief  Networker_ABC
*/
// Created: NLD 2006-09-19
// =============================================================================
class Networker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
     Networker_ABC( Dispatcher& dispatcher );
    ~Networker_ABC();
    //@}

    //! @name Main
    //@{
    void Update();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Networker_ABC( const Networker_ABC& );            //!< Copy constructor
    Networker_ABC& operator=( const Networker_ABC& ); //!< Assignement operator
    //@}

protected:
    Dispatcher&     dispatcher_;
    DIN::DIN_Engine dinEngine_;
};

}

#endif // __Networker_ABC_h_
