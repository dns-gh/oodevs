// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __DummyModelObserver_h_
#define __DummyModelObserver_h_

#include "ModelObserver_ABC.h"

namespace gui
{

// =============================================================================
/** @class  DummyModelObserver
    @brief  DummyModelObserver
*/
// Created: JSR 2012-09-25
// =============================================================================
class DummyModelObserver : public ModelObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DummyModelObserver() {}
    virtual ~DummyModelObserver() {}
    //@}

public:
    //! @name Operations
    //@{
    virtual void OnRename( const kernel::SafePointer< kernel::Entity_ABC >& /*entity*/, const QString& /*newName*/ ) {}
    virtual void CreateCommunication() {}
    virtual void CreateTeam() {}
    virtual void ClearSelection() {}
    virtual void CreateFormation( int /*level*/ ) {}
    virtual kernel::Automat_ABC* ReplaceAutomat( kernel::Entity_ABC& /*original*/, const kernel::AutomatType& /*type*/ ) { return 0; }
    virtual void DeleteEntity( const kernel::Entity_ABC& /*entity*/ ) {}
    virtual void CreateCityOrDistrict( kernel::Entity_ABC* /*parent*/ ) {}
    virtual void DeleteBlocks( const std::vector< const kernel::UrbanObject_ABC* >& /*urbanObjects*/ ) {}
    //@}
};

} //! namespace gui

#endif // __DummyModelObserver_h_
