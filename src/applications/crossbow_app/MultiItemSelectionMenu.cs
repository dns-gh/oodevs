using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.Framework;

namespace Crossbow
{
    #region MultiItemSelectionMenu
    [ComVisible(true)]
    [Guid("1909EFBF-A5DF-470e-A60C-4450FEEAB33D")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("Crossbow.MultiItemSelectionMenu")]
    public class MultiItemSelectionMenu : IMultiItem
    {     
        private System.Collections.ArrayList m_items = new System.Collections.ArrayList();
        private OrderHandler m_orderProxy;

        public MultiItemSelectionMenu()
        {            
            CSwordExtension extension = Tools.GetCSwordExtension();
            m_orderProxy = extension.OrderHandler;            
        }

        #region COM Registration Function(s)
        [ComRegisterFunction()]
        [ComVisible(false)]
        static void RegisterFunction(Type registerType)
        {
            // Required for ArcGIS Component Category Registrar support
            ArcGISCategoryRegistration(registerType);
        }

        [ComUnregisterFunction()]
        [ComVisible(false)]
        static void UnregisterFunction(Type registerType)
        {
            // Required for ArcGIS Component Category Registrar support
            ArcGISCategoryUnregistration(registerType);
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
        
        public void Add(string name)
        {
           Add(name, null);
        }

        public void Add(string name, IOrderParameter value)
        {
            m_items.Add(new KeyValuePair<string, IOrderParameter>(name, value));
        }

        #region IMultiItem Members

        public string Caption
        {
            get { return m_orderProxy.CurrentMessage; }
        }

        public int HelpContextID
        {
            get { throw new Exception("The method or operation is not implemented."); }
        }

        public string HelpFile
        {
            get { throw new Exception("The method or operation is not implemented."); }
        }

        public string Message
        {
            get { return m_orderProxy.CurrentMessage; }
        }

        public string Name
        {
            get { return "ItemSelectionMenu"; }
        }

        public void OnItemClick(int index)
        {
            if (index >= 0 && index < m_items.Count)
            {
                KeyValuePair<string, IOrderParameter> item = (KeyValuePair<string, IOrderParameter>)m_items[index];
                if (item.Value == null)
                    m_orderProxy.CreateOrder(item.Key);
                else
                    m_orderProxy.SelectParameter(item.Value, item.Key);
            }
        }

        public int OnPopup(object hook)
        {
            m_items.Clear();
            m_orderProxy.OnContextMenu(this);
            return m_items.Count;
        }

        public int get_ItemBitmap(int index)
        {
            return 4; // $$$$ JCR:
        }

        public string get_ItemCaption(int index)
        {
            return ((KeyValuePair<string, IOrderParameter>)m_items[index]).Key;
        }

        public bool get_ItemChecked(int index)
        {
            return false;
        }

        public bool get_ItemEnabled(int index)
        {
            return true;
        }

        #endregion
    }
    #endregion
}
