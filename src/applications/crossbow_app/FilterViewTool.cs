using System;
using System.Drawing;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.SystemUI;
using System.Windows.Forms;

namespace Crossbow
{
    /// <summary>
    /// Summary description for FilterViewTool.
    /// </summary>
    [ComVisible(true)]
    [Guid("f44a3523-8fae-4fd8-9895-fa0bde4efff5")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("CSword.FilterView")]
    public class FilterViewTool : BaseCommand, IToolControl, IDisposable
    {
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
            MxCommands.Register(regKey);

        }
        /// <summary>
        /// Required method for ArcGIS Component Category unregistration -
        /// Do not modify the contents of this method with the code editor.
        /// </summary>
        private static void ArcGISCategoryUnregistration(Type registerType)
        {
            string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
            MxCommands.Unregister(regKey);

        }

        #endregion
        #endregion

        private System.Windows.Forms.ComboBox m_teamCombo;
        private bool m_disposed;

        public FilterViewTool()
        {
            base.m_category = "CSword"; //localizable text 
            base.m_caption = "Filter view";  //localizable text 
            base.m_message = "Filter view to show situation from one team's point of view";  //localizable text
            base.m_toolTip = "Filter view";  //localizable text
            base.m_name = "CSword_FilterView";   //unique id, non-localizable (e.g. "MyCategory_ArcMapTool")
            try
            {
                string bitmapResourceName = GetType().Name + ".bmp";
                base.m_bitmap = new Bitmap(GetType(), bitmapResourceName);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.Message, "Invalid Bitmap");
            }
        }

        ~FilterViewTool()
        {
            Dispose(false);
        }

        #region Overriden Class Methods

        /// <summary>
        /// Occurs when this tool is created
        /// </summary>
        /// <param name="hook">Instance of the application</param>
        public override void OnCreate(object hook)
        {
            //Disable if it is not ArcMap
            base.m_enabled = hook is IMxApplication;
            InitControl();
        }

        public override bool Enabled
        {
            get
            {
                if (m_teamCombo != null)
                    m_teamCombo.Refresh();
                return base.Enabled;
            }
        }

        /// <summary>
        /// Occurs when this tool is clicked
        /// </summary>
        public override void OnClick()
        {
            // NOTHING
        }
        #endregion

        #region IToolControl Members

        public bool OnDrop(esriCmdBarType barType)
        {
            return true;
        }

        public void OnFocus(ICompletionNotify complete)
        {
            // NOTHING
        }

        public int hWnd
        {
            get
            {
                if (m_teamCombo != null)
                    return m_teamCombo.Handle.ToInt32();
                else
                    return 0;
            }
        }

        #endregion

        public void InitControl()
        {
            if (!base.m_enabled)
                return;
            m_teamCombo = new System.Windows.Forms.ComboBox();
            m_teamCombo.DropDownStyle = ComboBoxStyle.DropDownList;
            m_teamCombo.Width = 150;
            m_teamCombo.SelectedIndexChanged += new System.EventHandler(OnSelectionChanged);
            int index = m_teamCombo.Items.Add("All");
            m_teamCombo.Items.Add( "Friends" );
            m_teamCombo.Items.Add( "Hostiles" );
            m_teamCombo.SelectedIndex = index;
        }

        private void OnSelectionChanged(object sender, System.EventArgs e)
        {
            string filter = m_teamCombo.SelectedItem.ToString();
            IMxDocument mxDocument = Tools.GetMxDocument();
            IFeatureLayer layer = Tools.GetIFeatureLayerFromLayerName(Tools.GetCSwordExtension().Config.LayersConfiguration.Units);
            if (layer != null)
            {
                FilterLayer(layer, filter);
                mxDocument.ActiveView.Refresh();
            }
        }

        public void FilterLayer(IFeatureLayer layer, string filter)
        {
            if (layer == null)
                return;
            IFeatureLayerDefinition definition = (IFeatureLayerDefinition)layer;
            definition.DefinitionExpression = "";
            if (filter != "All" && filter.Length > 0)
                definition.DefinitionExpression = "Symbol_ID like '?" + filter[0] + "*'";
        }

        #region IDisposable Members

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool usercall)
        {
            if (!m_disposed)
            {
                m_disposed = true;
                if (usercall)
                    m_teamCombo.Dispose();
            }
        }

        #endregion
    }
}
