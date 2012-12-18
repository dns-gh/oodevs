// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "ENT/ENT_Enums_Gen.h"

namespace kernel
{
    class Controller;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace xml
{
    class xistream;
}

namespace tools
{
    class IdManager;
}

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: SBO 2006-09-19
// =============================================================================
class Formation : public kernel::EntityImplementation< kernel::Formation_ABC >
                , public kernel::Extension_ABC
                , public kernel::Drawable_ABC
                , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( kernel::Controller& controller, E_NatureLevel level, tools::IdManager& idManager );
             Formation( xml::xistream& xis, kernel::Controller& controller, tools::IdManager& idManager );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual QString GetBasicName() const;
    virtual E_NatureLevel GetLevel() const;
    virtual void Rename( const QString& name );
    void SetLevel( E_NatureLevel level );
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const  kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void InitializeSymbol() const;
    bool IsAggregated( const kernel::Entity_ABC& entity ) const;
    bool HasAggregatedSubordinate() const;
    //@}

private:
    //! @name Member data
    //@{
    E_NatureLevel level_;
    mutable std::string symbolPath_;
    mutable std::string levelPath_;
    mutable float verticalOffset_;
    //@}
};

#endif // __Formation_h_
