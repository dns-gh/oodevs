using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.Geodatabase;

namespace Crossbow
{
    [ComVisible(true)]
    [Guid("e8fc9be2-8b39-4250-8522-6d061cfc5769")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("Crossbow.InfoPanel")]
    public partial class InfoPanel : UserControl, IDockableWindowDef
    {
        private IFeatureLayerSelectionEvents_FeatureLayerSelectionChangedEventHandler m_selectionChangedEvent;
        private IFeatureLayer m_unitLayer;
        private IEnumFeature m_selection;

        #region COM Registration Function(s)
        [ComRegisterFunction()]
        [ComVisible(false)]
        static void RegisterFunction(Type registerType)
        {
            // Required for ArcGIS Component Category Registrar support
            ArcGISCategoryRegistration(registerType);
            //
            // TODO: Add any COM registration code here
            //
        }

        [ComUnregisterFunction()]
        [ComVisible(false)]
        static void UnregisterFunction(Type registerType)
        {
            // Required for ArcGIS Component Category Registrar support
            ArcGISCategoryUnregistration(registerType);

            //
            // TODO: Add any COM unregistration code here
            //
        }

        #region ArcGIS Component Category Registrar generated code
        /// <summary>
        /// Required method for ArcGIS Component Category registration -
        /// Do not modify the contents of this method with the code editor.
        /// </summary>
        private static void ArcGISCategoryRegistration(Type registerType)
        {
            string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
            MxDockableWindows.Register(regKey);

        }
        /// <summary>
        /// Required method for ArcGIS Component Category unregistration -
        /// Do not modify the contents of this method with the code editor.
        /// </summary>
        private static void ArcGISCategoryUnregistration(Type registerType)
        {
            string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
            MxDockableWindows.Unregister(regKey);

        }

        #endregion
        #endregion

        public InfoPanel()
        {
            InitializeComponent();
        }

        #region IDockableWindowDef Members

        string IDockableWindowDef.Caption
        {
            get
            {
                return "Information panel";
            }
        }

        int IDockableWindowDef.ChildHWND
        {
            get { return this.Handle.ToInt32(); }
        }

        string IDockableWindowDef.Name
        {
            get
            {
                return this.Name;
            }
        }

        void IDockableWindowDef.OnCreate(object hook)
        {
            Tools.GetCSwordExtension().ModelLoaded += new EventHandler(OnModelLoadedHandler);
        }

        void IDockableWindowDef.OnDestroy()
        {
            m_selectionChangedEvent = null;
            m_unitLayer = null;
            m_selection = null;
        }

        private void OnModelLoadedHandler(object sender, EventArgs e)
        {
            if (m_selectionChangedEvent == null)
            {
                m_unitLayer = (IFeatureLayer)Tools.GetLayerByName(Tools.GetCSwordExtension().Config.LayersConfiguration.Units + " - Graphics");
                //m_unitLayer = Tools.GetIFeatureLayerFromLayerName(Tools.GetCSwordExtension().Config.LayersConfiguration.Units + " - Graphics");
                if (m_unitLayer == null)
                    return; // $$$$ SBO 2007-08-28: display error message or something
                m_selection = (IEnumFeature)m_unitLayer;
                m_selectionChangedEvent = new IFeatureLayerSelectionEvents_FeatureLayerSelectionChangedEventHandler(OnSelectionChanged);
                ((IFeatureLayerSelectionEvents_Event)m_unitLayer).FeatureLayerSelectionChanged += m_selectionChangedEvent;
            }
        }

        private void OnSelectionChanged()
        {
            ClearSelection();
            if (m_unitLayer != null && m_selection != null)
                UpdateSelection();
        }

        private void ClearSelection()
        {
            reportsList.Items.Clear();
        }

        private void UpdateSelection()
        {
            m_selection.Reset();
            IFeature selected = m_selection.Next();
            if (selected == null)
                return;
            IFeatureWorkspace workspace = Tools.RetrieveWorkspace(m_unitLayer);
            ITable reportsTable = workspace.OpenTable(Tools.GetCSwordExtension().Config.LayersConfiguration.Reports);
            reportsList.BeginUpdate();
            IQueryFilter query = new QueryFilterClass();
            query.WhereClause = "unit_id=" + Tools.GetValue<int>(selected, "Public_OID");
            ICursor cursor = reportsTable.Search(query, false);
            for (IRow result = cursor.NextRow(); result != null; result = cursor.NextRow() )
                reportsList.Items.Add(Tools.GetValue<string>(result, "message"));
            reportsList.EndUpdate();
        }

        object IDockableWindowDef.UserData
        {
            get { return this; }
        }

        #endregion
    }
}
