// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PreferencePanel_ABC_h_
#define __PreferencePanel_ABC_h_

namespace gui
{

// =============================================================================
/** @class  PreferencePanel_ABC
    @brief  PreferencePanel_ABC
*/
// Created: SBO 2007-01-03
// =============================================================================
class PreferencePanel_ABC : public QScrollView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PreferencePanel_ABC( QWidget* parent, const char* name = 0 );
    virtual ~PreferencePanel_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Commit() {}
    virtual void Reset() {}
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PreferencePanel_ABC( const PreferencePanel_ABC& );            //!< Copy constructor
    PreferencePanel_ABC& operator=( const PreferencePanel_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void insertChild ( QObject* pObj );
    virtual QLayout* layout();
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    QVBox* pBox_;
    //@}
};

}

#endif // __PreferencePanel_ABC_h_
