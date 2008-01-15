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
using ESRI.ArcGIS.DataSourcesGDB;
using System.Xml;

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
            
            string rootDir = System.IO.Path.GetDirectoryName(m_workspace.ExerciseFile) + "\\sessions";
            foreach (string dir in System.IO.Directory.GetDirectories(rootDir) )
            {
                int index = dir.LastIndexOf('\\');
                if ( index < dir.Length )
                    sessionList.Items.Add(dir.Substring(index + 1, dir.Length - index - 1));
            }
        }

        private void sessionList_SelectedIndexChanged(object sender, EventArgs e)
        {
            string item = (string)sessionList.SelectedItem;
            string sessionDir = System.IO.Path.GetDirectoryName(m_workspace.ExerciseFile) + "/sessions/" + item;
            XmlDocument doc = new XmlDocument();
            doc.Load(sessionDir + "/session.xml");
            string workspace = doc.SelectSingleNode("/session/config/dispatcher/plugins/plugin[@name='crossbow']/parameter[@name=\"geodatabase\"]/@value").Value;
            LookupLayer( System.IO.Path.GetDirectoryName(m_workspace.ExerciseFile) + "/" + workspace );
            string shared = doc.SelectSingleNode("/session/config/dispatcher/plugins/plugin[@name='crossbow']/parameter[@name=\"geodatabase-shared\"]/@value").Value;
            m_workspace.SharedFile = System.IO.Path.GetDirectoryName(m_workspace.ExerciseFile) + "/" + shared;
        }

        private void LookupLayer(string file)
        {
            m_workspace.WorkspaceFile = file;
            IWorkspaceFactory factory = new AccessWorkspaceFactoryClass();
            IWorkspace workspace = factory.OpenFromFile(file, 0);
            if (workspace != null)
            {
                layersGrid.SelectedObject = null;
                FeatureLayerList.SetValues(workspace.get_DatasetNames(esriDatasetType.esriDTFeatureClass));
                layersGrid.SelectedObject = m_workspace.LayersConfiguration;
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