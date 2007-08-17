using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ESRI.ArcGIS.Catalog;
using ESRI.ArcGIS.CatalogUI;
using ESRI.ArcGIS.Geodatabase;

namespace Crossbow
{
    public partial class WorkspaceConfigurationForm : Form
    {
        private WorkspaceConfiguration m_workspace;

        public WorkspaceConfigurationForm(WorkspaceConfiguration config)
        {
            InitializeComponent();
            m_workspace = config;
        }

        private void exerciseFileButton_Click(object sender, EventArgs e)
        {
            exerciseFileDialog.ShowDialog(this);
        }

        private void exerciseFileDialog_FileOk(object sender, CancelEventArgs e)
        {
            exerciseFileText.Text = exerciseFileDialog.FileName;
            m_workspace.ExerciseFile = exerciseFileDialog.FileName;
        }

        private void databaseButton_Click(object sender, EventArgs e)
        {
            GxDialog openDialog = new GxDialog();
            openDialog.Title = "Open dataset";
            openDialog.ObjectFilter = new GxFilterPersonalGeodatabases();
            IEnumGxObject selection;
            if (openDialog.DoModalOpen(Handle.ToInt32(), out selection))
            {
                IGxObject element;
                while ((element = selection.Next()) != null)
                {
                    databaseText.Text = element.FullName;
                    m_workspace.WorkspaceFile = element.FullName;
                    IGxDatabase database = element as IGxDatabase;
                    if (database != null)
                    {
                        layersGrid.SelectedObject = null;
                        FeatureLayerList.SetValues( database.Workspace.get_DatasetNames(esriDatasetType.esriDTFeatureClass) );
                        layersGrid.SelectedObject = m_workspace.LayersConfiguration;
                    }
                }
            }
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            m_workspace.NotifyLoaded();
            Hide();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Hide();
        }
    }
}