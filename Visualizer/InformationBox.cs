using System.Windows.Forms;

namespace Visualizer
{
    public partial class InformationBox : Form
    {
        public InformationBox()
        {
            InitializeComponent();
            textBox1.ReadOnly = true;
        }

        public void SetText(string text)
        {
            textBox1.Text = text;
        }
    }
}