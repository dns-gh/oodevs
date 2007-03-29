// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalStates_h_
#define __MedicalStates_h_

#include "Types.h"
#include "LogisticHierarchies.h"

// =============================================================================
/** @class  MedicalStates
    @brief  MedicalStates
*/
// Created: SBO 2006-10-24
// =============================================================================
class MedicalStates : public LogisticHierarchies< MedicalSuperior, kernel::MedicalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             MedicalStates( kernel::Controller& controller, kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dico );
    virtual ~MedicalStates();
    //@}

    //! @name Operations
    //@{
    void SetSuperior( const MedicalSuperior& automat );
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MedicalStates( const MedicalStates& );            //!< Copy constructor
    MedicalStates& operator=( const MedicalStates& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner );
    //@}
};

#endif // __MedicalStates_h_
