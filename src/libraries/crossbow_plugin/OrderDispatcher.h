// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_OrderDispatcher_h_
#define __crossbow_OrderDispatcher_h_

class Publisher_ABC;

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Workspace_ABC;
    class Row_ABC;
    class ActionSerializer_ABC;

// =============================================================================
/** @class  OrderDispatcher
    @brief  OrderDispatcher
*/
// Created: SBO 2007-05-31
// =============================================================================
class OrderDispatcher
{
public:
    //! @name Constructors/Destructor
    //@{
             OrderDispatcher( const dispatcher::Model_ABC&, Workspace_ABC& workspace, ActionSerializer_ABC& serializer );
    virtual ~OrderDispatcher();
    //@}

    //! @name Operations
    //@{
    bool IsValidOrder( const Row_ABC& row ) const;
    void Dispatch( Publisher_ABC& publisher, const Row_ABC& row );
    void DispatchFragOrder( Publisher_ABC& publisher, unsigned long targetId, const Row_ABC& row );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderDispatcher( const OrderDispatcher& );            //!< Copy constructor
    OrderDispatcher& operator=( const OrderDispatcher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Clean();
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    Workspace_ABC& workspace_;
    ActionSerializer_ABC& serializer_;
    //@}
};

}
}

#endif // __crossbow_OrderDispatcher_h_
