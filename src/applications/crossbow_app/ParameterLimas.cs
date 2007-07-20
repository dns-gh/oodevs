using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public sealed class ParameterLimas : IOrderParameter
    {
        private OrderManager m_orderManager;

        public void OnCreate(OrderManager hook)
        {
            m_orderManager = hook;
        }

        public void Serialize(ITable table, int id)
        {
        }

        public void OnContextMenu(ICommandBar cmdBar, int x, int y, IFeature selected)
        {

        }
        public string Name
        {
            get
            {
                return "limas";
            }
        }
    }
}
