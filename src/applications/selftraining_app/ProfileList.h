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

#include <vector>

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

namespace xml
{
    class xistream;
}

namespace frontend
{
    class Profile;
}

// =============================================================================
/** @class  ProfileList
    @brief  ProfileList
*/
// Created: RDS 2008-09-05
// =============================================================================
class ProfileList : public QListWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ProfileList( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader );
    virtual ~ProfileList();
    //@}

public slots:
    //! @name Slots
    //@{
    void Update( const tools::Path& exercise );
    void OnSelect( int index );
    //@}

signals:
    //! @name
    //@{
    void Select( const frontend::Profile& profile );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadProfiles( const tools::Path& exercise );
    void ReadProfile ( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC& fileLoader_;
    std::vector< frontend::Profile > profiles_;
    //@}
};

#endif // __ProfileList_h_
