using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Geodatabase;

namespace crossbow
{
    public sealed class ParameterDirection
    {
        private OrderManager m_orderManager;
        private int m_direction;

        public void OnCreate(OrderManager hook)
        {
            m_orderManager = hook;
        }

        public void Serialize(ITable table, int id)
        {            
            IRow row = table.CreateRow();

            Tools.SetValue(row, "order_id", id);
            Tools.SetValue(row, "name", Name);
            Tools.SetValue(row, "context", true);
            Tools.SetValue(row, "ParamValue", m_direction);
            row.Store();
        }

        public string Name
        {
            get
            {
                return "direction";
            }
        }
    }
}
