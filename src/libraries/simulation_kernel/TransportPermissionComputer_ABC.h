// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_TransportPermissionComputer_ABC_H__
#define __transport_TransportPermissionComputer_ABC_H__

namespace transport
{
// =============================================================================
/** @class  TransportPermissionComputer_ABC
    @brief  TransportPermissionComputer_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class TransportPermissionComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportPermissionComputer_ABC() {}
    virtual ~TransportPermissionComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void AllowLoading( bool doAllow ) = 0;
    //@}
};

}

#endif /* __transport_TransportPermissionComputer_ABC_H__ */
