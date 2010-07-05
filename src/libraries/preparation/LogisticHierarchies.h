// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticHierarchies_h_
#define __LogisticHierarchies_h_

#include "LogisticHierarchies_ABC.h"
#include "clients_kernel/LogisticHierarchies.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"

// =============================================================================
/** @class  LogisticHierarchies
    @brief  LogisticHierarchies
*/
// Created: SBO 2006-10-24
// =============================================================================
template< typename Superior, typename I >
class LogisticHierarchies : public LogisticHierarchies_ABC< I >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity );
    virtual ~LogisticHierarchies();
    //@}

    //! @name Operations
    //@{
    using LogisticHierarchies_ABC< I >::Load;
    void Load( xml::xistream& xis, const kernel::Automat_ABC* superior );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticHierarchies( const LogisticHierarchies& );            //!< Copy constructor
    LogisticHierarchies& operator=( const LogisticHierarchies& ); //!< Assignment operator
    //@}

protected:
    //! @name Helpers
    //@{
    void SetSuperior( const Superior& automat );
    virtual void SetSuperiorInternal( kernel::Entity_ABC* superior );
    void DrawLink( const geometry::Point2f& where, const kernel::GlTools_ABC& tools, float curve, bool displayLinks, bool displayMissings ) const;
    //@}

protected:
    //! @name Member data
    //@{
    Superior tc2_;
    //@}
};

#include "LogisticHierarchies.inl"

#endif // __LogisticHierarchies_h_
