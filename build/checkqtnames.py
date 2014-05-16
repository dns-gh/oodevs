import sys, os, re

def parseqtnames(path):
    for line in file(path):
        line = line.strip()
        if not line:
            continue
        named, name, parentname = line.split(',', 2)
        named = int(named)
        if not named:
            yield name, parentname

ignoredleaves = set([
    "ADN_Graph",
    "FilterDialogs",
    "LoadableField",
    "LocationEditorBox",
    "LongNameEditor",
    "Menu",
    "Q3ButtonGroup",
    "Q3Canvas",
    "Q3GroupBox",
    "Q3HButtonGroup",
    "Q3HGroupBox",
    "Q3PopupMenu",
    "Q3VButtonGroup",
    "Q3VGroupBox",
    "QComboBoxPrivateContainer",
    "QFrame",
    "QHeaderView",
    "QMenu",
    "QObject",
    "QPropertyAnimation",
    "QProgressBar",
    "QScrollArea",
    "QScrollBar",
    "QShortcut",
    "QSignalMapper",
    "QSizeGrip",
    "QSplitter",
    "QStatusBar",
    "QTableCornerButton",
    "QToolBox",
    "SuccessFactorList",
    "SymbolSizeOptionChooser",
    "UserProfileList",
    "gui::AggregateToolbar",
    "gui::ButtonGroup",
    "gui::ConsistencyDialog_ABC",
    "gui::DrawingCategoryItem",
    "gui::ExpandableGroupBox",
    "gui::GQ_Plot",
    "gui::HelpSystem",
    "gui::LocationEditorBox",
    "gui::NatureEditionCategory",
    "gui::NumericLimitsEditor_ABC",
    "gui::ObjectPrototype_ABC",
    "gui::OptionMenuBase",
    "gui::Panels",
    "gui::RichView_ABC",
    "gui::SymbolIcons",
    "gui::TerrainPicker",
    "gui::UnitPreviewIcon",
    "gui::DisplayExtractor",
    "gui::LinkInterpreter_ABC",
    'Q3HBox',
    'Q3VBox',
    'QBoxLayout',
    'QGridLayout',
    'QHBoxLayout',
    'QLabel',
    'QProcess',
    'QTimer',
    'QVBoxLayout',
    'QWidget',
    ])

ignorednodes = set([
    "ADN_Graph",
    "QComboBoxPrivateContainer",
    "QLabel",
    "QTabBar",
    "QTabWidget",
    "QTextEdit",
    "gui::GQ_Plot", # waiting to be refactored
    "gui::RichLabel",
    ])
reigndesc = '(' + '|'.join(re.escape(p) for p in ignorednodes) + ')'
reigndesc = reigndesc + '\.'
reigndesc = re.compile(reigndesc)

retype = re.compile(r'(Layout|Delegate|Model|Factory|Validator|Action|'
    + r'Separator|Widget|Handler|Panel|Dialog|Editor)$')

if __name__ == '__main__':
    path = sys.argv[1]
    leaves = set()
    unnamed = set()
    for name, parentname in parseqtnames(path):
        if reigndesc.search(name):
            continue
        leaf = name.rsplit('.', 1)[-1]
        if leaf in ignoredleaves:
            continue
        if retype.search(name):
            continue
        leaves.add(leaf)
        unnamed.add( name + " in " + parentname if len(parentname) > 0 else name)
    ret = 0
    if unnamed:
        w = sys.stderr.write
        for name in unnamed:
            w('error: ui element is unnamed: %s\n' % name)
        ret = 1
    sys.exit(ret)
