// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SideList_h_
#define __SideList_h_

#include <qlistbox.h>

namespace tools
{
    class GeneralConfig;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  SideList
    @brief  SideList
*/
// Created: SBO 2008-02-22
// =============================================================================
class SideList : public QListBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SideList( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~SideList();
    //@}

public slots:
    //! @name Slots
    //@{
    void Update( const QString& exercise );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SideList( const SideList& );            //!< Copy constructor
    SideList& operator=( const SideList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void UpdateSides( const std::string& orbat );
    void ReadSide( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    //@}
};

#endif // __SideList_h_
