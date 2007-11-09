// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Intelligence_h_
#define __Intelligence_h_

#include "Types.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class HierarchyLevel_ABC;
}

class IdManager;

// =============================================================================
/** @class  Intelligence
    @brief  Intelligence
*/
// Created: SBO 2007-10-12
// =============================================================================
class Intelligence : public kernel::EntityImplementation< kernel::Intelligence_ABC >
                   , public kernel::Extension_ABC
                   , public kernel::Drawable_ABC
                   , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Intelligence( kernel::Controller& controller, IdManager& idManager, const std::string& symbol, const kernel::HierarchyLevel_ABC& level, bool embarked, const kernel::Karma& karma );
             Intelligence( kernel::Controller& controller, IdManager& idManager, xml::xistream& xis, const kernel::Resolver_ABC< kernel::HierarchyLevel_ABC, QString >& levels );
    virtual ~Intelligence();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Intelligence( const Intelligence& );            //!< Copy constructor
    Intelligence& operator=( const Intelligence& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& ic );
    void CreateDictionary( kernel::Controller& controller );
    virtual const kernel::Karma& GetKarma() const;
    virtual std::string GetSymbol() const;
    virtual bool IsEmbarked() const;
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
    virtual void SerializeIntelligences( xml::xostream& ) const;

    virtual void Delete();
    virtual void Rename( const QString& name );
    void SetKarma( const IntelligenceKarma& karma );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    std::string symbol_;
    const kernel::HierarchyLevel_ABC* level_;
    IntelligenceKarma karma_;
    bool embarked_;
    //@}
};

#endif // __Intelligence_h_
