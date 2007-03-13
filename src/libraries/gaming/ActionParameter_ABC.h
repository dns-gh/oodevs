// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameter_ABC_h_
#define __ActionParameter_ABC_h_

// =============================================================================
/** @class  ActionParameter_ABC
    @brief  ActionParameter_ABC
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameter_ABC();
    virtual ~ActionParameter_ABC();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetId() const;
    virtual QString GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameter_ABC( const ActionParameter_ABC& );            //!< Copy constructor
    ActionParameter_ABC& operator=( const ActionParameter_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    //@}
};

#endif // __ActionParameter_ABC_h_
