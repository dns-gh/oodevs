using System;
using System.Drawing;
using System.Runtime.InteropServices;
using ESRI.ArcGIS.ADF.BaseClasses;
using ESRI.ArcGIS.ADF.CATIDs;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.ArcMapUI;
using System.Windows.Forms;

namespace Sword
{
    namespace Crossbow
    {
        /// <summary>
        /// Summary description for OrderTool.
        /// </summary>
        [ComVisible(true)]
        [Guid("4e5dd04c-e711-467c-a21c-e7f250f0991f")]
        [ClassInterface(ClassInterfaceType.None)]
        [ProgId("Sword.Crossbow.OrderTool")]
        public sealed class OrderTool : BaseTool
        {
            #region COM Registration Function(s)
            [ComRegisterFunction()]
            [ComVisible(false)]
            static void RegisterFunction(Type registerType)
            {
                // Required for ArcGIS Component Category Registrar support
                ArcGISCategoryRegistration(registerType);
                //Registry.Register(registerType, //Registry.COMPONENT_MX_COMMAND);
            }

            [ComUnregisterFunction()]
            [ComVisible(false)]
            static void UnregisterFunction(Type registerType)
            {
                // Required for ArcGIS Component Category Registrar support
                ArcGISCategoryUnregistration(registerType);
                //Registry.Unregister(registerType, //Registry.COMPONENT_MX_COMMAND);
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
                GMxCommands.Register(regKey);
            }
            /// <summary>
            /// Required method for ArcGIS Component Category unregistration -
            /// Do not modify the contents of this method with the code editor.
            /// </summary>
            private static void ArcGISCategoryUnregistration(Type registerType)
            {
                string regKey = string.Format("HKEY_CLASSES_ROOT\\CLSID\\{{{0}}}", registerType.GUID);
                MxCommands.Unregister(regKey);
                GMxCommands.Unregister(regKey);
            }

            #endregion
            #endregion

            public OrderTool()
            {
                base.m_category = "Sword"; //localizable text
                base.m_caption = "Create order";  //localizable text
                base.m_message = "Manage order command";  //localizable text 
                base.m_toolTip = "Manage order command";  //localizable text 
                base.m_name = "Sword_Crossbow_OrderTool";   //unique id, non-localizable (e.g. "MyCategory_ArcMapCommand")
                try
                {
                    //
                    // TODO: change resource name if necessary
                    //
                    string bitmapResourceName = GetType().Name + ".bmp";
                    base.m_bitmap = new Bitmap(GetType(), bitmapResourceName);
                    base.m_cursor = new System.Windows.Forms.Cursor(GetType(), GetType().Name + ".cur");
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Trace.WriteLine(ex.Message, "Invalid Bitmap");
                }
            }

            #region Overriden Class Methods

            /// <summary>
            /// Occurs when this tool is created
            /// </summary>
            /// <param name="hook">Instance of the application</param>
            public override void OnCreate(object hook)
            {
                if (Tools.IsSupportedApplication(hook))
                    Tools.GetCSwordExtension().ModelLoaded += new EventHandler(OnModelLoaded);
                m_enabled = false;
            }

            void OnModelLoaded(object sender, EventArgs e)
            {
                m_enabled = true;
            }

            public override bool OnContextMenu(int X, int Y)
            {
                OrderHandler handler = Tools.GetCSwordExtension().OrderHandler;
                if (handler != null)
                    return handler.OnContextMenu(X, Y);
                return false;
            }

            #endregion
        }
    }
}