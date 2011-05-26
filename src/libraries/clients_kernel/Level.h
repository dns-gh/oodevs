// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Level_h_
#define __Level_h_

#include "HierarchyLevel_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  Level
    @brief  Level
*/
// Created: SBO 2006-09-21
// =============================================================================
class Level : public HierarchyLevel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Level( const QString& name, const HierarchyLevel_ABC* next );
    virtual ~Level();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetId() const;
    virtual QString GetName() const;

    virtual const HierarchyLevel_ABC* GetPrevious() const;
    virtual const HierarchyLevel_ABC* GetNext() const;
    //@}

    //! @name Operations
    //@{
    void SetPrevious( const HierarchyLevel_ABC& previous );
    virtual bool operator<( const HierarchyLevel_ABC& rhs ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Level( const Level& );            //!< Copy constructor
    Level& operator=( const Level& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    QString name_;
    std::string symbol_;
    const HierarchyLevel_ABC* previous_;
    const HierarchyLevel_ABC* next_;
    //@}
};

}

#endif // __Level_h_
