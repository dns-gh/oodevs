using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public class Order
    {
        private IFeature m_selectedFeature;
        private ParameterLimits m_limits = new ParameterLimits();
        private ParameterLimas m_limas = new ParameterLimas();
        private ParameterDirection m_direction = new ParameterDirection();
        private System.Collections.Hashtable m_parameters = new System.Collections.Hashtable();
        private IMissionForm m_handler = null;
        private int m_OID;

        public string Name
        {
            get
            {
                return m_handler.OrderName;
            }
        }
        public int OID
        {
            get
            {
                return m_OID;
            }
            set
            {
                m_OID = value;
            }
        }
        
        public Order( IMissionForm handler )
        {
            m_handler = handler;                        
        }

        public void OnCreate(OrderFactory factory)
        {
            factory.BuildParameters(this);
        }

        public void RegisterParameter(string name, string typeID)
        {
            OrderParameter param = new OrderParameter(name, typeID);                        
            
            m_handler.BindParameter(param);
            m_parameters.Add(name, param);
        }

        public void OnContextMenu(ICommandBar menu, int x, int y, IFeature selected)
        {
            m_limits.OnContextMenu(menu, x, y, selected);
            m_limas.OnContextMenu(menu, x, y, selected);

            foreach (object obj in m_parameters )
                ((IOrderParameter)obj).OnContextMenu(menu, x, y, selected);    
        }

        public void Serialize(IFeatureWorkspace featureWorkspace)
        {
            ITable table = featureWorkspace.OpenTable("OrdersParameters");
            m_limits.Serialize(table, m_OID);
            m_limas.Serialize(table, m_OID);

            foreach (object obj in m_parameters)
                ((IOrderParameter)obj).Serialize(table, m_OID);
        }

        public void Execute()
        {
        }

    }
}
