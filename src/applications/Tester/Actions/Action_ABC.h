// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Action_ABC_h_
#define __Action_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

namespace TEST {

    class Scheduler;

// =============================================================================
/** @class  Action_ABC
    @brief  Action_ABC
    @par    Using example
    @code
    Action_ABC;
    @endcode
*/
// Created: SBO 2005-08-04
// =============================================================================
class Action_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Action_ABC();
    virtual ~Action_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool Run( const Scheduler& scheduler ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual       bool         IsReady() const;
    virtual const std::string& GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Action_ABC( const Action_ABC& );            //!< Copy constructor
    Action_ABC& operator=( const Action_ABC& ); //!< Assignment operator
    //@}

protected:
    //! @name Action validation and commit
    //@{
    virtual bool CheckParameters() const;
    virtual void Serialize      () = 0;
    virtual void Send           ( const Scheduler& scheduler ) = 0;
    virtual void Clean          () = 0;
    //@}

protected:
    //! @name Member data
    //@{
    std::string strName_;
    //@}
};

} // end namespace TEST

#include "Action_ABC.inl"

#endif // __Action_ABC_h_
