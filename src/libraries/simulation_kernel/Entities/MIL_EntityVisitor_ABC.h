// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_EntityVisitor_ABC_h_
#define __MIL_EntityVisitor_ABC_h_

// =============================================================================
/** @class  MIL_EntityVisitor_ABC
    @brief  Entity visitor base class
*/
// Created: SBO 2006-02-24
// =============================================================================
template< typename T >
class MIL_EntityVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_EntityVisitor_ABC() {}
    virtual ~MIL_EntityVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const T& element ) = 0;
    //@}
};

class MIL_Formation;
class MIL_Automate;
class MIL_AgentPion;

class MIL_EntitiesVisitor_ABC
{
public:
    virtual bool Visit( const MIL_Formation& formation ) = 0;
    virtual bool Visit( const MIL_Automate& automat) = 0;
    virtual void Visit( const MIL_AgentPion& unit ) = 0;
};

#endif // __MIL_EntityVisitor_ABC_h_
