// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Intelligence_h_
#define __Intelligence_h_

#include "game_asn/Asn.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Formation_ABC;
    class HierarchyLevel_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  Intelligence
    @brief  Intelligence
*/
// Created: SBO 2007-10-17
// =============================================================================
class Intelligence : public kernel::EntityImplementation< kernel::Intelligence_ABC >
                   , public kernel::Extension_ABC
                   , public kernel::Drawable_ABC
                   , public kernel::Updatable_ABC< ASN1T_MsgIntelligenceUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             Intelligence( const ASN1T_MsgIntelligenceCreation& message, kernel::Controller& controller
                         , const kernel::Resolver_ABC< kernel::Formation_ABC >& formations
                         , const kernel::Resolver_ABC< kernel::HierarchyLevel_ABC >& levels
                         , Publisher_ABC& publisher);
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
    void CreateDictionary( kernel::Controller& controller );
    virtual const kernel::Karma& GetKarma() const;
    virtual std::string GetSymbol() const;
    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
    virtual bool IsEmbarked() const;
    virtual void Delete();
    virtual void Rename( const QString& name );
    virtual void DoUpdate( const ASN1T_MsgIntelligenceUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Resolver_ABC< kernel::HierarchyLevel_ABC >& levels_;
    const kernel::Formation_ABC& formation_;
    std::string symbol_;
    const kernel::HierarchyLevel_ABC* level_;
    const kernel::Karma* karma_;
    bool embarked_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __Intelligence_h_
