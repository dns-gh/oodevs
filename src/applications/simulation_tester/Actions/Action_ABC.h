// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Action_ABC_h_
#define __Action_ABC_h_

namespace TEST {

    class Scheduler;

// =============================================================================
/** @class  Action_ABC
    @brief  Action base class
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
    virtual std::string ToString() const = 0;
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
};

} // end namespace TEST

#endif // __Action_ABC_h_
