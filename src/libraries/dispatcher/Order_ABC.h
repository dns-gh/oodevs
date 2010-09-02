// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Order_ABC_h_
#define __Order_ABC_h_

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  Order_ABC
    @brief  Order_ABC
*/
// Created: NLD 2007-04-20
// =============================================================================
class Order_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Order_ABC( unsigned long id );
    virtual ~Order_ABC();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    //@}

    //! @name Operations
    //@{
    virtual void Send( ClientPublisher_ABC& publisher ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Order_ABC( const Order_ABC& );            //!< Copy constructor
    Order_ABC& operator=( const Order_ABC& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    //@}
};

}

#endif // __Order_ABC_h_
