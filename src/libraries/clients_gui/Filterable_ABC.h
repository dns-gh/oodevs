// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui_Filterable_ABC_h
#define __gui_Filterable_ABC_h

namespace gui
{
    class Filter_ABC;
    class RichView_ABC;
    class StandardModel;

// =============================================================================
/** @class  Filterable_ABC
    @brief  Filterable_ABC definition
*/
// Created: ABR 2014-04-25
// =============================================================================
class Filterable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Filterable_ABC() {}
    virtual ~Filterable_ABC() {}
    //@}

    //! @name Abstract Operation
    //@{
    virtual void ApplyFilters( const std::map< int, std::vector< std::shared_ptr< Filter_ABC > > >& filters ) = 0;
    virtual void CreateFilters( RichView_ABC& richView ) = 0;
    virtual void Purge() = 0;
    virtual QHeaderView* GetHeader() const = 0;
    //@}

private:
    //! @name Member
    //@{
    StandardModel* model_;
    //@}
};

} //! namespace gui

#endif // __gui_Filterable_ABC_h
