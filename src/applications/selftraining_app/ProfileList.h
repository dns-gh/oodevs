// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ProfileList_h_
#define __ProfileList_h_

#include "clients_kernel/Resolver.h" 

#include <qiconview.h>

namespace tools
{
    class GeneralConfig ; 
}

namespace xml
{
    class xistream ; 
}

class Side; 

// =============================================================================
/** @class  ProfileList
    @brief  ProfileList
*/
// Created: RDS 2008-09-05
// =============================================================================
class ProfileList : public QListBox 
{

    Q_OBJECT ; 

public:
    //! @name Constructors/Destructor
    //@{
             ProfileList( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~ProfileList();
    //@}

public slots:

    //! @name Slots
    //@{
    void Update( const QString& exercise );
    //@}

signals:

    //! @name 
    //@{
    void Select    ( const QString& profile ) ;
    void Highlight ( const QString& profile ) ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfileList( const ProfileList& );            //!< Copy constructor
    ProfileList& operator=( const ProfileList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadSides   ( const std::string& exercise ) ; 
    void ReadSide    ( xml::xistream& xis ) ; 
    void ReadProfiles( const std::string& exercise ) ; 
    void ReadProfile ( xml::xistream& xis ); 
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_ ; 
    kernel::Resolver< Side > sides_ ; 
    //@}
};

#endif // __ProfileList_h_
