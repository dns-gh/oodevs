// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanObject_ABC_h_
#define __UrbanObject_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
class ObjectTypes;
class UrbanTemplateType;

// =============================================================================
/** @class  UrbanObject_ABC
    @brief  Urban Object
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanObject_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject_ABC();
    virtual ~UrbanObject_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetTypeName() const;
    virtual QString GetName() const = 0;
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const GraphicalEntity_ABC* >& elements ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const;
    typedef UrbanTemplateType* UrbanTemplateTypePtr;
    virtual void ApplyTemplate( const UrbanTemplateTypePtr& urbanTemplate ) = 0;
    virtual void UpdateTemplate( const kernel::ObjectTypes& objectTypes ) = 0;
    virtual bool IsUpdatingTemplate() const = 0;
    //@}
};

}

#endif // __UrbanObject_ABC_h_
