import sys, os, re

def parseqtnames(path):
    for line in file(path):
        named, name = line.split(None, 1)
        named = int(named)
        if not named:
            yield name.strip()

ignored = set([
    'QBoxLayout',
    'QVBoxLayout',
    'QHBoxLayout',
    'QTimer',
    'QLabel',
    'QProcess',
    'Q3VBox',
    'Q3HBox',
    'QWidget',
    'QGridLayout',
    "QScrollBar",
    "QSignalMapper",
    "QScrollArea",
    "QHeaderView",
    "QProgressBar",
    "Q3Canvas",
    "LongNameEditor",
    "gui::GQ_Plot",
    "QShortcut",
    "QFrame",
    "LoadableField",
    "gui::ButtonGroup",
    "Q3ButtonGroup",
    "Q3VButtonGroup",
    "Q3HButtonGroup",
    "LocationEditorBox",
    "QObject",
    "AffinitiesDialog",
    "gui::Panels",
    "QStatusBar",
    "SuccessFactorList",
    "UserProfileList",
    "QSplitter",
    "gui::OptionMenuBase",
    "Q3PopupMenu",
    "FilterDialogs",
    "gui::AggregateToolbar",
    "gui::SearchTreeView_ABC",
    "gui::HelpSystem",
    "gui::LocationEditorBox",
    "gui::ExpandableGroupBox",
    "QToolBox",
    "gui::TerrainPicker",
    "QSizeGrip",
    "gui::SymbolIcons",
    "SymbolSizeOptionChooser",
    "gui::UnitPreviewIcon",
    "gui::NumericLimitsEditor_ABC",
    "gui::NatureEditionCategory",
    "gui::DrawingCategoryItem",
    "gui::ObjectPrototype_ABC",
    "gui::ConsistencyDialog_ABC",
    "Menu",
    "QMenu",
    ])

ignoredescendants = set([
    "QSpinBox",
    "QLineEdit",
    "QTextEdit",
    "QComboBox",
    "QCheckBox",
    "QTabWidget",
    "QTableWidget",
    "QTableView",
    "QListWidget",
    "QListView",
    "QToolButton",
    "QPushButton",
    "QRadioButton",
    "QdateTimeEdit",
    "QTimeEdit",
    "QTreeWidget",
    "Q3GroupBox",
    "QGroupBox",
    "QLabel",
    "gui::GQ_Plot",
    "gui::CheckComboBox",
    "gui::RichLabel",
    "gui::RichGroupBox",
    "QTabBar",
    "gui::RichLineEdit",
    ])
reigndesc = '(' + '|'.join(re.escape(p) for p in ignoredescendants) + ')'
reigndesc = reigndesc + '\.'
reigndesc = re.compile(reigndesc)

retype = re.compile(r'(Layout|Delegate|Model|Factory|Validator|Action|'
    + r'Separator|Widget|Handler|Panel|Dialog|Editor)$')

if __name__ == '__main__':
    path = sys.argv[1]
    leaves = set()
    unnamed = set()
    for name in parseqtnames(path):
        if reigndesc.search(name):
            continue
        leaf = name.rsplit('.', 1)[-1]
        if leaf in ignored:
            continue
        if retype.search(name):
            continue
        leaves.add(leaf)
        unnamed.add(name)

    ret = 0
    if unnamed:
        w = sys.stderr.write
        for name in sorted(unnamed):
            w('error: ui element is unnamed: %s\n' % name)
        ret = 1
    sys.exit(ret)
