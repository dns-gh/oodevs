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

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace xml
{
    class xistream;
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
             Intelligence( kernel::Controller& controller, IdManager& idManager, const std::string& symbol, const std::string& level, const kernel::Karma& karma );
             Intelligence( kernel::Controller& controller, IdManager& idManager, xml::xistream& xis );
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
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string symbol_;
    const std::string level_;
    const kernel::Karma& karma_;
    //@}
};

#endif // __Intelligence_h_
