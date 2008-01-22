using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Carto;

namespace Sword
{
    namespace Crossbow
    {
        public class OrderHandler : IMultiItemContextMenuHandler
        {
            private IFeature m_targetFeature;
            private int m_targetX;
            private int m_targetY;
            private Order m_order;
            private OrderFactory m_orderFactory;

            public OrderHandler(StaticModel model)
            {
                m_orderFactory = new OrderFactory(model);
            }

            #region Selection management
            public IFeature TargetFeature
            {
                get
                {
                    return m_targetFeature;
                }
            }

            public void Reset()
            {
                if (m_order == null)
                    return;
                m_order.Dispose();
                m_order = null;
            }

            private bool SelectFeature(int x, int y)
            {
                UpdateSelection(Tools.GetDocument().FeatureSelection);
                m_targetX = x; m_targetY = y;
                return m_order != null || (m_targetFeature != null && m_targetFeature.Class.FindField("Public_OID") > 0); // m_order.NeedContextMenu()
            }

            private void UpdateSelection(ISelection selection)
            {
                IEnumFeature pEnumFeature = selection as IEnumFeature;
                pEnumFeature.Reset();
                m_targetFeature = pEnumFeature.Next();
            }
            #endregion

            #region Context menu management
            public bool OnContextMenu(int x, int y)
            {
                if (!SelectFeature(x, y))
                    return false;
                CreateMissionContextMenu();
                return true;
            }

            private static void CreateMissionContextMenu()
            {
                ICommandBar menu = Tools.GetIDocument().CommandBars.Create("OrderMenu", esriCmdBarType.esriCmdBarTypeShortcutMenu);
                UID uid = new UIDClass();
                uid.Value = "Sword.Crossbow.OrderSelectionMenu";
                object Missing = Type.Missing;
                menu.Add(uid, ref Missing);
                if (menu.Count > 0)
                    menu.Popup(0, 0);
            }
            #endregion

            #region IMultiItemContextMenuHandler Members

            public void BuildContextMenu(MultiItemContextMenu menu)
            {
                if (m_order != null)
                    m_order.OnContextMenu(menu, m_targetX, m_targetY, m_targetFeature);
                else
                    m_orderFactory.BuildMissionContextMenu(menu, m_targetFeature);
            }

            public void ActivateItem(string name, object value)
            {
                if (value == null)
                    m_order = m_orderFactory.CreateOrder(name, this);
                else
                {
                    if (m_order == null)
                        return;
                    m_order.SelectParameter(value as IOrderParameter, name);
                }
            }

            public string GetCurrentMessage()
            {
                if (m_order != null)
                    return "Select parameter for " + m_order.Name + ".";
                else
                    return "Select unit mission.";
            }

            #endregion
        }
    }
}