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
#include "ENT/ENT_Enums.h"

class StaticModel;

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
    //! @name Types
    //@{
    typedef std::pair< std::string, geometry::Point2f >     T_Child;
    typedef std::vector< T_Child >                          T_Children;
    typedef T_Children::const_iterator                    CIT_Children;
    //@}

public:
    //! @name Static
    //@{
    static const std::string typeName_;
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
    virtual const std::string& GetLevel() const = 0;
    virtual const std::string& GetSymbol() const = 0;
    virtual const std::string GetLevelSymbol() const = 0;
    virtual const std::string& GetNature() const = 0;
    virtual bool IsConverted() const = 0;
    virtual void UpdateSymbol( const std::string& level, const std::string& nature, const std::string& symbol ) = 0;
    virtual void SerializeGhostAttributes( xml::xostream& ) const = 0;
    virtual void ReadGhostAttributes( xml::xistream& xis ) = 0;
    virtual void Finalize( const StaticModel& staticModel ) = 0;
    virtual const T_Children& GetChildren() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetTypeName() const;

    virtual void Select( ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const;
    virtual void Activate( ActionController& controller ) const;
    virtual void OverFly( ActionController& controller ) const;
    //@}
};

}

#endif // __Ghost_ABC_h_
