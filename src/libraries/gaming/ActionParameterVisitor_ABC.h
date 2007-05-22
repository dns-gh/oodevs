// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterVisitor_ABC_h_
#define __ActionParameterVisitor_ABC_h_

class ActionParameterObstacle;
class ActionParameterLima;
class ActionParameterLimit;
class ActionParameterLocation;
class ActionParameterPath;
class ActionParameterPathPoint;

// =============================================================================
/** @class  ActionParameterVisitor_ABC
    @brief  ActionParameterVisitor_ABC
*/
// Created: SBO 2007-05-22
// =============================================================================
class ActionParameterVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterVisitor_ABC() {}
    virtual ~ActionParameterVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const ActionParameterObstacle&  ) {}
    virtual void Visit( const ActionParameterLima&      ) {}
    virtual void Visit( const ActionParameterLimit&     ) {}
    virtual void Visit( const ActionParameterPath&      ) {}
    virtual void Visit( const ActionParameterPathPoint& ) {}
    virtual void Visit( const ActionParameterLocation&  ) {}
    //@}
};

#endif // __ActionParameterVisitor_ABC_h_
