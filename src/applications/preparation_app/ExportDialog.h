// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExportDialog_h_
#define __ExportDialog_h_

#include <boost/ptr_container/ptr_vector.hpp>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

class ExportFilter;

// =============================================================================
/** @class  ExportDialog
    @brief  ExportDialog
*/
// Created: SBO 2010-04-22
// =============================================================================
class ExportDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ExportDialog( QWidget* parent, const tools::ExerciseConfig& config );
    virtual ~ExportDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectFilter( int index );
    void OnAccept();
    void OnReject();
    void OnBrowse();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExportDialog( const ExportDialog& );            //!< Copy constructor
    ExportDialog& operator=( const ExportDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void LoadFilters( const std::string& filename );
    void ReadFilter( xml::xistream& xis );
    virtual QSize sizeHint() const;
    //@}

    //! @name Types
    //@{
    typedef boost::ptr_vector< ExportFilter > T_Filters;
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    T_Filters filters_;
    QListBox* list_;
    QLabel* description_;
    QLineEdit* output_;
    QPushButton* okButton_;
    //@}
};

#endif // __ExportDialog_h_
