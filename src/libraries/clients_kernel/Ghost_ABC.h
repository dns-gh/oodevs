// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Ghost_ABC_h_
#define __Ghost_ABC_h_

#include "Entity_ABC.h"
#include "ENT/ENT_Enums_Gen.h"

namespace kernel
{

// =============================================================================
/** @class  Ghost_ABC
    @brief  Ghost_ABC
*/
// Created: ABR 2011-10-14
// =============================================================================
class Ghost_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Ghost_ABC();
    virtual ~Ghost_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual const QString& GetType() const = 0;
    virtual E_GhostType GetGhostType() const = 0;
    virtual const std::string& GetSymbol() const = 0;
    virtual const std::string GetLevelSymbol() const = 0;
    virtual const std::string& GetNature() const = 0;
    virtual bool IsConverted() const = 0;
    virtual void UpdateSymbol( const std::string& level, const std::string& nature, const std::string& symbol ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    virtual void OverFly( ActionController& controller ) const;
    //@}
};

}

#endif // __Ghost_ABC_h_
