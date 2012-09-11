// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ModelObserver_ABC_h_
#define __ModelObserver_ABC_h_

namespace kernel
{
    class Automat_ABC;
    class AutomatType;
    class Entity_ABC;
}

namespace gui
{

// =============================================================================
/** @class  ModelObserver_ABC
    @brief  ModelObserver_ABC
*/
// Created: JSR 2012-08-31
// =============================================================================
class ModelObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelObserver_ABC() {}
    virtual ~ModelObserver_ABC() {}
    //@}

public:
    //! @name Operations
    //@{
    virtual void OnRename( kernel::Entity_ABC& entity, const QString& newName ) = 0;
    virtual void CreateCommunication() = 0;
    virtual void CreateTeam() = 0;
    virtual void ClearSelection() = 0;
    virtual void CreateFormation( int level ) = 0;
    virtual kernel::Automat_ABC* ReplaceAutomat( kernel::Entity_ABC& original, const kernel::AutomatType& type ) = 0;
    //@}
};

} //! namespace gui

#endif // __ModelObserver_ABC_h_
