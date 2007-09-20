using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.Framework;

namespace Crossbow
{
    #region IMultiItemContextMenuHandler
    public interface IMultiItemContextMenuHandler
    {
        void BuildContextMenu(MultiItemContextMenu menu);
        void ActivateItem(string name, object value);
        string GetCurrentMessage();
    }
    #endregion

    #region MultiItemContextMenu
    public class MultiItemContextMenu : IMultiItem
    {
        private ArrayList m_items = new ArrayList();
        private IMultiItemContextMenuHandler m_handler;

        public MultiItemContextMenu(IMultiItemContextMenuHandler handler)
        {
            m_handler = handler;
        }

        public void Add(string name, object value)
        {
            m_items.Add(new KeyValuePair<string, object>(name, value));
        }

        #region IMultiItem Members

        public string Caption
        {
            get { return m_handler.GetCurrentMessage(); }
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
            get { return m_handler.GetCurrentMessage(); }
        }

        public string Name
        {
            get { return "ItemSelectionMenu"; }
        }

        public void OnItemClick(int index)
        {
            if (index >= 0 && index < m_items.Count)
            {
                KeyValuePair<string, object> item = (KeyValuePair<string, object>)m_items[index];
                m_handler.ActivateItem(item.Key, item.Value);
            }
        }

        public int OnPopup(object hook)
        {
            m_items.Clear();
            m_handler.BuildContextMenu(this);
            return m_items.Count;
        }

        public int get_ItemBitmap(int index)
        {
            return 4; // $$$$ JCR:
        }

        public string get_ItemCaption(int index)
        {
            return ((KeyValuePair<string, object>)m_items[index]).Key;
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


    #region OrderSelectionMenu
    [ComVisible(true)]
    [Guid("1909EFBF-A5DF-470e-A60C-4450FEEAB33D")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("Crossbow.OrderSelectionMenu")]
    public class OrderSelectionMenu : MultiItemContextMenu
    {     
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

        public OrderSelectionMenu()
            : base(Tools.GetCSwordExtension().OrderHandler)
        {
            // NOTHING
        }
    }
    #endregion

    #region EmergencyCreationMenu
    [ComVisible(true)]
    [Guid("968BD0CA-4BA6-4ab0-8CAE-EB624B6FF5ED")]
    [ClassInterface(ClassInterfaceType.None)]
    [ProgId("Crossbow.EmergencyCreationMenu")]
    public class EmergencyCreationMenu : MultiItemContextMenu
    {
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

        public EmergencyCreationMenu()
            : base(Tools.GetCSwordExtension().EmergencyHandler)
        {
            // NOTHING
        }
    }
    #endregion
}
