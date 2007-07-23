using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;

namespace crossbow
{
    public interface IMissionForm
    {
        string OrderName { get; }
        void BindParameter(IOrderParameter param);        
    }

    public partial class MissionOrderForm : Form, IMissionForm
    {
        private OrderManager m_orderManager = null;

        public MissionOrderForm(string name)
        {
            OrderName = name;

            CSwordExtension extension = Tools.GetCSwordExtension();
            m_orderManager = extension.OrderManager;
            m_orderManager.BindUI(this);

            InitializeComponent();
            m_UnitName.Text = m_orderManager.SelectionName();
        }

        #region IMissionForm implementation
        public void BindParameter(IOrderParameter param)
        {
            if (m_ParameterTree != null) // $$$$ SBO 2007-07-23: 
                m_ParameterTree.Nodes.Add(param.Name);
        }
        
        public string OrderName
        {
            get
            {
                return base.Text;
            }

            set
            {
                base.Text = value;
            }
        }
        #endregion
    }
}