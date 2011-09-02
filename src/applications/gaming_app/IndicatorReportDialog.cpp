// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "IndicatorReportDialog.h"
#include "moc_IndicatorReportDialog.cpp"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/LinkInterpreter_ABC.h"
#include "gaming/Score.h"
#include "gaming/ScoreModel.h"
#include "gaming/Tools.h"
#include "tools/ExerciseConfig.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <fstream>

// -----------------------------------------------------------------------------
// Name: IndicatorReportDialog constructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
IndicatorReportDialog::IndicatorReportDialog( QWidget* parent, const ScoreModel& model, const tools::ExerciseConfig& config, gui::LinkInterpreter_ABC& interpreter )
    : QDialog( parent, "IndicatorReportDialog" )
    , model_      ( model )
    , config_     ( config )
    , interpreter_( interpreter )
{
    setCaption( tools::translate( "IndicatorReports", "Create report" ) );
    setFixedWidth( 400 );
    QVBoxLayout* vBox = new QVBoxLayout();
    vBox->setContentsMargins( 5, 5, 5, 5 );
    setLayout( vBox );
    {
        QGroupBox* box = new QGroupBox( tools::translate( "IndicatorReports", "Template" ) );
        QLabel* label = new QLabel( tools::translate( "IndicatorReports", "File: " ) );
        templateFile_ = new QLineEdit();
        templateFile_->setReadOnly( true );
        QPushButton* browse = new QPushButton( tools::translate( "IndicatorReports", "Browse..." ) );
        connect( browse, SIGNAL( clicked() ), SLOT( OnBrowseTemplate() ) );
        connect( templateFile_, SIGNAL( textChanged( const QString& ) ), SLOT( OnFileChanged() ) );

        QHBoxLayout* boxLayout = new QHBoxLayout();
        boxLayout->addWidget( label );
        boxLayout->addWidget( templateFile_, 1 );
        boxLayout->addWidget( browse );
        box->setLayout( boxLayout );

        vBox->addWidget( box );
    }
    {
        QGroupBox* box = new QGroupBox( tools::translate( "IndicatorReports", "Output" ) );
        QLabel* label = new QLabel( tools::translate( "IndicatorReports", "File: " ) );
        outputFile_ = new QLineEdit();
        QPushButton* browse = new QPushButton( tools::translate( "IndicatorReports", "Browse..." ) );
        displayReport_ = new QCheckBox( tools::translate( "IndicatorReports", "Show generated report" ) );
        displayReport_->setChecked( true );
        connect( browse, SIGNAL( clicked() ), SLOT( OnBrowseOutput() ) );
        connect( outputFile_, SIGNAL( textChanged( const QString& ) ), SLOT( OnFileChanged() ) );

        QGridLayout* boxLayout = new QGridLayout();
        boxLayout->setColumnStretch( 1, 1 );
        boxLayout->addWidget( label, 0, 0 );
        boxLayout->addWidget( outputFile_, 0, 1 );
        boxLayout->addWidget( browse, 0, 2 );
        boxLayout->addWidget( displayReport_, 1, 1 );
        box->setLayout( boxLayout );

        vBox->addWidget( box );
    }
    {
        ok_ = new QPushButton( tools::translate( "IndicatorReports", "Ok" ) );
        ok_->setEnabled( false );
        QPushButton* cancel = new QPushButton( tools::translate( "IndicatorReports", "Cancel" ) );
        connect( ok_, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( reject() ) );
        
        QHBoxLayout* boxLayout = new QHBoxLayout();
        boxLayout->addStretch( 1 );
        boxLayout->addWidget( ok_ );
        boxLayout->addWidget( cancel );

        vBox->addLayout( boxLayout );
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorReportDialog destructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
IndicatorReportDialog::~IndicatorReportDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorReportDialog::OnBrowseTemplate
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void IndicatorReportDialog::OnBrowseTemplate()
{
    const QString startPath( config_.BuildExerciseChildFile( "reports/" ).c_str() );
    QString filename = Q3FileDialog::getOpenFileName( startPath, tools::translate( "IndicatorReports", "Report template (*.html)" ), topLevelWidget()
                                                    , 0, tools::translate( "IndicatorReports", "Load report template file" ) );
    if( filename.isEmpty() )
        return;
    if( filename.startsWith( "//" ) )
        filename.replace( "/", "\\" );
    templateFile_->setText( filename );
}

// -----------------------------------------------------------------------------
// Name: IndicatorReportDialog::OnBrowseOutput
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void IndicatorReportDialog::OnBrowseOutput()
{
    const QString startPath = templateFile_->text().isEmpty() ? config_.BuildExerciseChildFile( "reports/" ).c_str() : QDir( templateFile_->text() ).absPath();
    QString filename = Q3FileDialog::getSaveFileName( startPath, tools::translate( "IndicatorReports", "Report (*.html)" ), topLevelWidget()
                                                   , 0, tools::translate( "IndicatorReports", "Save report to" ) );
    if( filename == QString::null )
        return;
    if( !filename.endsWith( ".html" ) )
        filename += ".html";
    outputFile_->setText( filename );
}

// -----------------------------------------------------------------------------
// Name: IndicatorReportDialog::OnFileChanged
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void IndicatorReportDialog::OnFileChanged()
{
    ok_->setDisabled( outputFile_->text().isEmpty() || templateFile_->text().isEmpty() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorReportDialog::OnAccept
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void IndicatorReportDialog::OnAccept()
{
    if( outputFile_->text() == templateFile_->text() )
        if( QMessageBox::warning( this, tools::translate( "IndicatorsReports", "Warning" ), tools::translate( "IndicatorsReports", "The specified output file is the also the input template.\n Are you sure you want to overwrite the template file?" )
                                , QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
            return;
    CreateReport();
    if( displayReport_->isChecked() )
        DisplayReport();
    accept();
}

// -----------------------------------------------------------------------------
// Name: IndicatorReportDialog::CreateReport
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void IndicatorReportDialog::CreateReport() const
{
    std::ifstream input( templateFile_->text().ascii() );
    std::ofstream output( outputFile_->text().ascii() );
    std::string line;
    while( input.good() )
    {
        std::getline( input, line );
        output << ReplaceTags( line ) << std::endl;
    }
}

namespace
{
    class ReportDisplayer : public kernel::Displayer_ABC
    {
    public:
                 ReportDisplayer( const std::string& score, std::string& result ) : score_( score ), result_( result ) {}
        virtual ~ReportDisplayer() {}

        virtual void Hide() {}
        virtual void Clear() {}
        virtual kernel::Displayer_ABC& SubItem( const QString& name )
        {
            attribute_ = name;
            return *this;
        }
        virtual void StartDisplay() {}
        virtual void DisplayFormatted( const QString& formatted )
        {
            boost::replace_all( result_, "${" + score_ + "." + attribute_ + "}", formatted.ascii() );
        }
        virtual void EndDisplay() {}

    private:
        const std::string score_;
        std::string& result_;
        std::string attribute_;
    };
}

// -----------------------------------------------------------------------------
// Name: IndicatorReportDialog::ReplaceTags
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
std::string IndicatorReportDialog::ReplaceTags( const std::string& line ) const
{
    boost::regex rx( "\\$\\{([^}]+)\\}" );
    boost::sregex_iterator end;
    std::string result( line );
    for( boost::sregex_iterator it( line.begin(), line.end(), rx ); it != end; ++it )
        for( unsigned int i = 1; i < it->size(); ++i )
            if( (*it)[i].matched )
            {
                const std::string match( (*it)[i].first, (*it)[i].second );
                const std::string name = match.substr( 0, match.find_last_of( '.' ) );
                if( const Score* score = model_.Find( name.c_str() ) )
                {
                    ReportDisplayer displayer( name, result );
                    score->Display( displayer );
                }
            }
    return result;
}

// -----------------------------------------------------------------------------
// Name: IndicatorReportDialog::DisplayReport
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void IndicatorReportDialog::DisplayReport() const
{
    interpreter_.Interprete( outputFile_->text() );
}
