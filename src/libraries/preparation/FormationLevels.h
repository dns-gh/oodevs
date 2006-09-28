// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationLevels_h_
#define __FormationLevels_h_

namespace kernel
{
    class HierarchyLevel_ABC;
}

// =============================================================================
/** @class  FormationLevels
    @brief  FormationLevels
*/
// Created: SBO 2006-09-21
// =============================================================================
class FormationLevels
{

public:
    //! @name Constructors/Destructor
    //@{
             FormationLevels();
    virtual ~FormationLevels();
    //@}

    //! @name Accessors
    //@{
    const kernel::HierarchyLevel_ABC* Resolve( const QString& name ) const;
    const kernel::HierarchyLevel_ABC* Resolve( unsigned int id ) const;
    const kernel::HierarchyLevel_ABC* GetRoot() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FormationLevels( const FormationLevels& );            //!< Copy constructor
    FormationLevels& operator=( const FormationLevels& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Initialize();
    //@}

private:
    //! @name Member data
    //@{
    kernel::HierarchyLevel_ABC* root_;
    //@}
};

#endif // __FormationLevels_h_
